from __future__ import annotations
from typing_extensions import Self, TypeAlias
import os, subprocess, sys, re, requests, pickle, datetime, time, typing, json
from urllib.parse import unquote
from bs4 import BeautifulSoup
from requests.cookies import RequestsCookieJar

topDir = '/home/y-tanabe/proj/compprog'
def getTopDir() -> str:
    return topDir

loginInfo = os.environ['HOME'] + '/.compprog.json'

def warn(*msg, **opt) -> None:
    print(*msg, **opt, file=sys.stderr, flush=True)

debug = warn

def die(*msg) -> typing.NoReturn:
    warn('ERROR:', *msg)
    sys.exit(1)

topPage = 'https://atcoder.jp'

class UtilError(Exception):
    pass

PayloadType: TypeAlias = dict[str, str]

# The singleton pattern is used.
class MySession:

    _instance : MySession | None = None

    def __new__(cls) -> MySession:
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance

    def __init__(self) -> None:
        if not hasattr(self, '_initialized'):
            self.cookies_file = f'{topDir}/.cookies'
            # requests.Session() creates a new session.  Thus, the following call
            # should be done only ONCE.
            self.session = requests.Session()
            # Cookies will be regarded as stale if 10 days has passed
            if not os.path.exists(self.cookies_file):
                warn(f'Cookies file {self.cookies_file} does not exist.  Will create one.')
                self.write_cookie_file(RequestsCookieJar())
            with open(self.cookies_file, 'rb') as fp:
                self.session.cookies.update(pickle.load(fp))
            self._initialized = True

    # def do_login(self):
    #     url = 'https://atcoder.jp/login?continue=https%3A%2F%2Fatcoder.jp%2Fhome'
    #     loginPage = self.get_page(url)
    #     soup = BeautifulSoup(loginPage, 'lxml')
    #     sch = soup.find_all(attrs={'name': 'csrf_token'})
    #     if not sch:
    #         die(f'doLogin: Failed to find csrf_token')
    #     with open(loginInfo) as fp:
    #         dic = json.load(fp)
    #     r = self.post_data(url, dic['atcoder'], csrf_token=sch[0]['value'])
    #     time.sleep(0.5)
        
    def write_cookie_file(self, jar: RequestsCookieJar) -> None:
        with open(self.cookies_file, 'wb') as wfp:
            pickle.dump(jar, wfp)

    def check_status_code(self, r: requests.Response, url: str) -> None:
        r.encoding = 'UTF-8'
        if r.status_code != requests.codes.ok:
            with open('z.log', 'w', encoding='UTF-8') as wfp:
                print(r.text, file=wfp)
            die(f'Failed to get/post for {url}.  status_code = {r.status_code}.  '
                  'Response is stored in z.log')
        self.write_cookie_file(self.session.cookies)

    def get_page(self, url: str) -> str:
        r = self.session.get(url)
        self.check_status_code(r, url)
        return r.text

    def post_data(self, url: str, payload: PayloadType,
                  csrf_token : str | None = None) -> str:
        if csrf_token is None:
            # This code is fragile.
            # A csrf_token is sent in an input_hidden tag of a form in HTML.
            # A correct code should send this token with a post request.
            # Thus, this third parameter csrf_token should not be None and
            # the caller of this MySession.post_data method should parse
            # the HTML file.  (And it should check the HTML file has been
            # retrieved within the current session (i.e., requests.Session()).)
            # But it seems in AtCoder it is OK to send the csrf_token value
            # embedded in the value of the cookie named REVEL_SESSION, so 
            # csrf_token=None seems working.
            if (val := self.session.cookies.get('REVEL_SESSION')):
                if (mo := re.search(r'csrf_token%3A(.+?)%00', val)):
                    csrf_token = unquote(mo[1])
            if csrf_token is None:
                die('Failed to retrieve csrf_token from sess.cookies.')
        payload['csrf_token'] = csrf_token
        r = self.session.post(url, payload)
        self.check_status_code(r, url)
        return r.text

    def check_login(self) -> bool:
        with open(loginInfo) as fp:
            dic = json.load(fp)
            username = dic['atcoder']['username']
        jp_page = f'{topPage}/?lang=ja'
        cont = self.get_page(jp_page)
        soup = BeautifulSoup(cont, 'lxml')
        header = soup.find(id='header')
        if header:
            # debug('header found')
            def pat1():
                if not (hm := header.find(class_='header-mypage')):
                    return False
                for s in hm.stripped_strings:
                    if s == username:
                        return True
                return False
            def pat2():
                if not (hc := header.find(class_='header-control')):
                    return False
                for s in hc.stripped_strings:
                    if s == 'ログイン':
                        return True
                return False
            if pat1():
                return True
            elif pat2():
                return False
        else:
            # debug('header not found')    
            die('Failed to check login.  Unknown page pattern.')

def getPage(url: str) -> str:
    return MySession().get_page(url)

def postData(url: str, payload: PayloadType,
             csrf_token : str | None = None) -> str:
    return MySession().post_data(url, payload, csrf_token)

def checkLogin() -> bool:
    return MySession().check_login()

def writeCookieFile(jar: RequestsCookieJar) -> None:
    MySession().write_cookie_file(jar)

def in_file(id: str) -> str:
    return f'din_{id}.txt'
def exp_file(id: str) -> str:
    return f'dexp_{id}.txt'
def act_file(id: str) -> str:
    return f'dact_{id}.txt'
def id_in_fname(name: str) -> str | None:
    mo = re.match(r'd(?:in|exp|act)_(.*)\.txt$', name)
    if mo: return mo[1]
    else:  return None

_fDP = "./double-prec"

def get_prec(prec, rewrite):
    '''Get precision for comparing doubles.

The precision is taken from argument prec and the contents of
the file named "./double-prec", with higher preference in argument prec.
If rewrite is True, the file is overwritten with prec if it is not None.
'''
    if prec is None:
        if os.path.exists(_fDP):
            with open(_fDP) as fp:
                try:
                    n = float(fp.readline())
                except TypeError:
                    msg = f'{_fDP} does not have a number.'
                    raise UtilError(msg)
                if n <= 0:
                    msg = f'Contents of {_fDP} is not positive.'
                    raise UtilError(msg)
                return n
        else:
            return None
    else:
        if prec <= 0:
            raise UtilError('Specified precision is not positive.')
        if rewrite:
            with open(_fDP, 'w') as wfp:
                print(prec, file=wfp)
        return prec

def compare_answer(ans1, ans2, prec=None, ignoreSpace=False):
    '''Compare two answers.  

If prec is not None, it should be a positive integer
and ans1 and ans2 are regarded as double and regarded as equal
when the absolute error or the relative error is less than 10**(-prec).
If prec is None, they are compared as strings.  If ignoreSpace is True,
each line is compared with ignoring space.
'''
    # print("\nans1=", ans1, "ans2=", ans2, "prec=", prec,
    #       "ignoreSpace=", ignoreSpace, "\n", file=sys.stderr)

    if prec is None:
        if ignoreSpace:
            list1 = ans1.split()
            list2 = ans2.split()
            return list1 == list2
        else:
            return ans1 == ans2
    else:
        try:
            thr = 10**(-prec)
        except TypeError:
            raise UtilError("Precision should be a number.")
        if prec <= 0:
            raise UtilError("Precision should be positive.")
        ds1 = ans1.split()
        ds2 = ans2.split()
        if len(ds1) != len(ds2):
            return False
        for (a1, a2) in zip(ds1, ds2):
            if a1 == a2:
                continue
            try:
                d1 = float(a1)
                d2 = float(a2)
            except ValueError:
                return False
            ret = abs(d1 - d2) < thr or \
                  (d2 > 1.0 and abs((d1 - d2)/d2) < thr) 
            if not ret:
                return False
        return True

def print_answer(path):
    print(f'--{path}--')
    with open(path, encoding='utf-8') as fp:
        for line in fp:
            print(line, end='')

def run(cmd, input=None, output=None, show_stderr=False, timeout=None):
    '''Run a process.

Returns (returncode, result).
Argument input should be None, a string, or a file descriptor.
Argument output should be None or a file descriptor.  When output
is a file descriptor, the snd of the return value (result) is None, otherwise
it is the contnets of stdout.
'''
    try:
        cp = subprocess.run(
            cmd, encoding='UTF-8', input=input,
            stdout=output or subprocess.PIPE,
            stderr=None if show_stderr else subprocess.DEVNULL,
            timeout=timeout
        )
    except FileNotFoundError as e:
        die(f'Failed to exec "{cmd[0]}".  {str(e)}\nMaybe not in PATH?')
    rc = cp.returncode
    return (rc,
            cp.stdout if (output is None and rc == 0) else None)
        
