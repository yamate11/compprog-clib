#!/usr/bin/env python3

from __future__ import annotations

from dataclasses import dataclass, field
from typing import List, Optional
from bs4 import BeautifulSoup, Tag, NavigableString
import html
import re


@dataclass
class SplitCase:
    input_text: str
    output_text: str


@dataclass
class Sample:
    input_text: str
    output_text: str
    split_cases: List[SplitCase] = field(default_factory=list)


_CASE_CLASS_RE = re.compile(r"^test-example-line-(\d+)$")


def _ensure_trailing_newline(s: str) -> str:
    if s == "":
        return s
    return s if s.endswith("\n") else s + "\n"


def _normalize_newlines(s: str) -> str:
    return s.replace("\r\n", "\n").replace("\r", "\n")


def _inline_text(node) -> str:
    parts: List[str] = []

    def rec(x) -> None:
        if isinstance(x, NavigableString):
            parts.append(str(x))
            return
        if not isinstance(x, Tag):
            return
        if x.name == "br":
            parts.append("\n")
            return
        for ch in x.children:
            rec(ch)

    rec(node)
    return html.unescape("".join(parts)).replace("\xa0", " ")


def _extract_case_index(tag: Tag) -> Optional[int]:
    for c in tag.get("class", []):
        if c in ("test-example-line", "test-example-line-odd", "test-example-line-even"):
            continue
        m = _CASE_CLASS_RE.fullmatch(c)
        if m:
            return int(m.group(1))
    return None


def _striped_line_tags(pre: Tag) -> List[tuple[int, Tag]]:
    out: List[tuple[int, Tag]] = []
    for ch in pre.children:
        if not isinstance(ch, Tag):
            continue
        idx = _extract_case_index(ch)
        if idx is not None:
            out.append((idx, ch))
    return out


def _group_striped_lines_by_case_index(pre: Tag) -> Optional[List[List[str]]]:
    """
    新スタイルの <pre> 直下にある
    test-example-line-0, test-example-line-1, ...
    を case index ごとにまとめる。
    """
    pairs = _striped_line_tags(pre)
    if not pairs:
        return None

    groups: List[List[str]] = []
    current_idx: Optional[int] = None
    current_lines: List[str] = []

    for idx, tag in pairs:
        text = _normalize_newlines(_inline_text(tag))

        if current_idx is None:
            current_idx = idx
            current_lines = [text]
        elif idx == current_idx:
            current_lines.append(text)
        else:
            groups.append(current_lines)
            current_idx = idx
            current_lines = [text]

    if current_lines:
        groups.append(current_lines)

    return groups


def _join_lines(lines: List[str]) -> str:
    return "".join((line if line.endswith("\n") else line + "\n") for line in lines)


def _pre_to_text(pre: Tag) -> str:
    """
    pre 全体を文字列化する。
    新スタイルなら test-example-line-* を1行ずつ join する。
    旧スタイルなら普通に文字列化する。
    """
    striped_groups = _group_striped_lines_by_case_index(pre)
    if striped_groups is not None:
        all_lines: List[str] = []
        for g in striped_groups:
            all_lines.extend(g)
        return _join_lines(all_lines)

    s = _normalize_newlines(_inline_text(pre))
    return _ensure_trailing_newline(s)


def _parse_first_line_as_int(block: str) -> Optional[int]:
    lines = _normalize_newlines(block).splitlines()
    if not lines:
        return None
    first = lines[0].strip()
    if not re.fullmatch(r"\d+", first):
        return None
    return int(first)


def _split_cases_from_groups(
    input_text: str,
    output_text: str,
    input_groups: Optional[List[List[str]]],
    output_groups: Optional[List[List[str]]],
) -> List[SplitCase]:
    if not input_groups or not output_groups:
        return []

    t = _parse_first_line_as_int(input_text)
    if t is None or t <= 0:
        return []

    result: List[SplitCase] = []

    # よくある形:
    # input:  [ ["T"], case1, case2, ..., caseT ]
    # output: [ out1, out2, ..., outT ]
    if len(input_groups) == t + 1 and len(output_groups) == t:
        if len(input_groups[0]) == 1 and input_groups[0][0].strip() == str(t):
            for i in range(t):
                case_in = ["1"] + input_groups[i + 1]
                case_out = output_groups[i]
                result.append(
                    SplitCase(
                        input_text=_join_lines(case_in),
                        output_text=_join_lines(case_out),
                    )
                )
            return result

    # 別パターン:
    # input が case1..caseT のみで，case1 の先頭に T が埋まっている
    if len(input_groups) == t and len(output_groups) == t:
        first_group = input_groups[0]
        if first_group and first_group[0].strip() == str(t):
            for i in range(t):
                if i == 0:
                    case_in = ["1"] + first_group[1:]
                else:
                    case_in = ["1"] + input_groups[i]
                case_out = output_groups[i]
                result.append(
                    SplitCase(
                        input_text=_join_lines(case_in),
                        output_text=_join_lines(case_out),
                    )
                )
            return result

    return []


def extract_codeforces_samples(html_source: str) -> List[Sample]:
    soup = BeautifulSoup(html_source, "html.parser")

    # sample-test / sample-tests のどちらでも拾えるようにしておく
    sample_root = soup.select_one("div.sample-test")
    if sample_root is None:
        wrapper = soup.select_one("div.sample-tests")
        if wrapper is not None:
            sample_root = wrapper.select_one("div.sample-test")
    if sample_root is None:
        return []

    inputs = sample_root.select("div.input")
    outputs = sample_root.select("div.output")

    n = min(len(inputs), len(outputs))
    results: List[Sample] = []

    for i in range(n):
        in_pre = inputs[i].find("pre")
        out_pre = outputs[i].find("pre")
        if in_pre is None or out_pre is None:
            continue

        input_text = _pre_to_text(in_pre)
        output_text = _pre_to_text(out_pre)

        input_groups = _group_striped_lines_by_case_index(in_pre)
        output_groups = _group_striped_lines_by_case_index(out_pre)

        split_cases = _split_cases_from_groups(
            input_text=input_text,
            output_text=output_text,
            input_groups=input_groups,
            output_groups=output_groups,
        )

        results.append(
            Sample(
                input_text=input_text,
                output_text=output_text,
                split_cases=split_cases,
            )
        )

    return results

## for testing

def main():

    with open('test3.html') as fp:
        lst = fp.readlines()
        html_source = ''.join(lst)

    samples = extract_codeforces_samples(html_source)

    for idx, s in enumerate(samples, 1):
        print(f"=== sample {idx} ===")
        print("INPUT:")
        print(repr(s.input_text))
        print("OUTPUT:")
        print(repr(s.output_text))

        for j, tc in enumerate(s.split_cases, 1):
            print(f"--- split case {j} ---")
            print("IN :", repr(tc.input_text))
            print("OUT:", repr(tc.output_text))


if __name__ == "__main__":
    main()

