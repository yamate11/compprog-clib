例として，polynomial/log_of_formal_power_series を作るとする．

$ ./yosupo-prepare polynomial/log_of_formal_power_series

      注意: この yosupo-prepare を tools に置くと，compprog/clib/tools にパスが通っているので具合が悪い．

すると，tests/polynomial/log_of_formal_power_series ディレクトリが作成され，
そこに，c_skel.cc ができる．テストセットも作成されている．

c_skel.cc を適当に編集する．

$ make DEBUG=yes
または
$ make

問題無ければ c_t が作成される．

$ stest -p ./c_t

