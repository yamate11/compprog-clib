import os, subprocess, sys, re, requests, pickle, datetime, time, typing, json
from urllib.parse import unquote
from bs4 import BeautifulSoup

topDir = '/home/y-tanabe/proj/compprog'
def getTopDir():
    return topDir

loginInfo = os.environ['HOME'] + '/.compprog.json'

def warn(*msg, **opt):
    print(*msg, **opt, file=sys.stderr, flush=True)

debug = warn

def die(*msg) -> typing.NoReturn:
    warn('ERROR:', *msg)
    sys.exit(1)

class UtilError(Exception):
    pass


class MySession():

    @classmethod
    def the_instance(cls, force_create=False):
        if not hasattr(cls, 'instance') or force_create:
            cls.instance = cls(force_do_login=force_create)
        return cls.instance

    def __init__(self, force_do_login):
        self.cookies_file = f'{topDir}/.cookies'
        # requests.Session() creates a new session.  Thus, the following call
        # should be done only ONCE.
        self.session = requests.Session()
        # Cookies will be regarded as stale if 12 hours passes (too short?)
        lim = datetime.datetime.now().timestamp() - 60*60*12
        if force_do_login \
           or not os.path.exists(self.cookies_file) \
           or os.stat(self.cookies_file).st_mtime < lim:
            self.do_login()
        else:
            with open(self.cookies_file, 'rb') as fp:
                self.session.cookies.update(pickle.load(fp))
        
    def do_login(self):
        url = 'https://atcoder.jp/login?continue=https%3A%2F%2Fatcoder.jp%2Fhome'
        loginPage = self.get_page(url)
        soup = BeautifulSoup(loginPage, 'lxml')
        sch = soup.find_all(attrs={'name': 'csrf_token'})
        if not sch:
            die(f'doLogin: Failed to find csrf_token')
        with open(loginInfo) as fp:
            dic = json.load(fp)
        r = self.post_data(url, dic['atcoder'], csrf_token=sch[0]['value'])
        time.sleep(0.5)
        
    def check_status_code(self, r, url):
        r.encoding = 'UTF-8'
        if r.status_code != requests.codes.ok:
            with open('z.log', 'w', encoding='UTF-8') as wfp:
                print(r.text, file=wfp)
            die(f'Failed to get/post for {url}.  status_code = {r.status_code}.  '
                  'Response is stored in z.log')
        with open(self.cookies_file, 'wb') as wfp:
            pickle.dump(self.session.cookies, wfp)

    def get_page(self, url):
        r = self.session.get(url)
        self.check_status_code(r, url)
        return r.text

    def post_data(self, url, payload, csrf_token=None):
        if csrf_token is None:
            # This code is fragile.
            # A csrf_token is sent in an input_hidden tag of a form in HTML.
            # A correct code should send this token with a post request.
            # Thus, this third parameter csrf_token should not be None and
            # the caller of this MySession.post_data method should parse
            # the HTML file.  (And it should check the HTML file has been
            # retrieved within the current session (i.e., requests.Session()).)
            # But it seems in AtCoder it is OK to send the csrf_token value
            # in the cookies with other post data, so csrf_token=None seems
            # working....
            for c in self.session.cookies:
                mo = re.search(r'csrf_token%3A(.+?)%00', c.value)
                if mo: 
                    csrf_token = unquote(mo[1])
                    break
            if csrf_token is None:
                die('Failed to retrieve csrf_token from sess.cookies.')
        payload['csrf_token'] = csrf_token
        r = self.session.post(url, payload)
        self.check_status_code(r, url)
        return r.text

def getPage(url):
    return MySession.the_instance().get_page(url)

def postData(url, payload, csrf_token=None):
    return MySession.the_instance().post_data(url, payload, csrf_token)

def forceLogin():
    MySession.the_instance(force_create=True)


def in_file(id): return f'din_{id}.txt'
def exp_file(id): return f'dexp_{id}.txt'
def act_file(id): return f'dact_{id}.txt'
def id_in_fname(name):
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
        
