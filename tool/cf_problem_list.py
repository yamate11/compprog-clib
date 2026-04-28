#!/usr/bin/env python3

from __future__ import annotations

from dataclasses import dataclass
from bs4 import BeautifulSoup
import re


@dataclass
class ContestProblemList:
    base_url: str
    problem_ids: list[str]


def extract_codeforces_contest_problems(html_source: str) -> ContestProblemList:
    soup = BeautifulSoup(html_source, "html.parser")

    # まず contest_id を HTML 全体から拾う。
    # /contest/2208
    # https://codeforces.com/contest/2208
    # contest/2208/problem/A
    # のようなものがどこかに含まれていればよい。
    contest_id = None
    m = re.search(r'(?:https?:)?//codeforces\.com/contest/(\d+)\b', html_source)
    if m:
        contest_id = m.group(1)
    else:
        m = re.search(r'(?<![A-Za-z0-9_])/contest/(\d+)\b', html_source)
        if m:
            contest_id = m.group(1)
        else:
            m = re.search(r'(?<![A-Za-z0-9_])contest/(\d+)\b', html_source)
            if m:
                contest_id = m.group(1)

    if contest_id is None:
        raise ValueError("contest id を HTML から抽出できませんでした")

    # まず href から問題 ID を拾う（相対/絶対 URL のゆれに強くする）
    href_re = re.compile(
        rf'^(?:https?:)?//codeforces\.com/contest/{contest_id}/problem/([A-Za-z][A-Za-z0-9]*)/?(?:\?.*)?$'
        rf'|^/contest/{contest_id}/problem/([A-Za-z][A-Za-z0-9]*)/?(?:\?.*)?$'
        rf'|^contest/{contest_id}/problem/([A-Za-z][A-Za-z0-9]*)/?(?:\?.*)?$'
    )

    problem_ids: list[str] = []
    seen: set[str] = set()

    for a in soup.find_all("a", href=True):
        href = a["href"].strip()
        m = href_re.fullmatch(href)
        if not m:
            continue
        pid = next(g for g in m.groups() if g is not None)
        if pid not in seen:
            seen.add(pid)
            problem_ids.append(pid)

    # href から取れなければ，Problems 表の 1 列目を使う
    if not problem_ids:
        # Codeforces の contest ページでは Problems 表の 1 列目に
        # A, B, C, D1, D2, ... が並ぶ
        for table in soup.find_all("table"):
            rows = table.find_all("tr")
            if not rows:
                continue

            candidate: list[str] = []
            for tr in rows:
                tds = tr.find_all("td")
                if len(tds) < 2:
                    continue

                first = tds[0].get_text(" ", strip=True)
                second = tds[1].get_text(" ", strip=True)

                # 1列目が問題IDらしく，2列目が空でない（問題名がある）行を拾う
                if re.fullmatch(r"[A-Za-z][A-Za-z0-9]*", first) and second:
                    candidate.append(first)

            if candidate:
                # 最初にそれらしい表を採用
                for pid in candidate:
                    if pid not in seen:
                        seen.add(pid)
                        problem_ids.append(pid)
                break

    if not problem_ids:
        raise ValueError("問題 ID を抽出できませんでした")

    base_url = f"https://codeforces.com/contest/{contest_id}/problem"
    return ContestProblemList(base_url=base_url, problem_ids=problem_ids)


## for testing

def main():
    with open('test4.html') as fp:
        lst = fp.readlines()
        html_source = ''.join(lst)

    res = extract_codeforces_contest_problems(html_source);

    print(res.base_url)
    print(res.problem_ids)


if __name__ == "__main__":
    main()
    
