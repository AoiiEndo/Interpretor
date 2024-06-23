# 環境
MacOS: Sonoma 14.5
gcc -v : Apple clang version 15.0.0 (clang-1500.3.9.4)
         Target: arm64-apple-darwin23.5.0
         Thread model: posix



# 実行方法
※gccにてC言語のコンパイルを行える環境必須
/プロジェクトディレクトリにて下記を実行
```
$ make
$ ./main ファイル名
$ make clean
```

# 概要
- file名は`test`
- 拡張子`lai`

# 参考
https://github.com/python/cpython                      #cpythonソースコード
https://nw.tsuda.ac.jp/lec/arm64/mac_m1/               #arm64アセンブリ
https://qiita.com/k0kubun/items/72d7e8c05eef14b00228   #arm64アセンブリ
https://3iz.jp/                                        #プログラミング言語の作り方(概要)
https://qiita.com/quwahara/items/09bd14ddc5de6d3f2780  #javaでインタプリタの実装
https://qiita.com/sawasaka/items/2df474fc00ec2105acab  #メモリの解放
https://qiita.com/Daiji256/items/09fa042e94907a232c57  #C言語でeval風関数の実装
