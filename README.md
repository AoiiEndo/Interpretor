# C言語によるインタプリタの作成

## 目次
- [環境](#環境)
    - [使用技術](#使用技術)
- [コマンド一覧](#コマンド一覧)
- [実行方法](#実行方法)
- [概要](#概要)
- [使用に関すること](#使用に関すること)
- [参考](#参考)


## 環境
MacOS: Sonoma 14.5  
gcc -v : Apple clang version 15.0.0 (clang-1500.3.9.4)  
         Target: arm64-apple-darwin23.5.0  
         Thread model: posix  
### 使用技術
<p style="display: inline">
<img src="https://img.shields.io/badge/-C-00599C.svg?logo=C&style=flat&logoColor=white">
<img src="https://img.shields.io/badge/-MacOS-000000.svg?logo=apple&style=flat&logoColor=white">
<img src="https://img.shields.io/badge/-Make-555555.svg?logo=gnu-make&style=flat&logoColor=white">
</p>

## コマンド一覧
| コマンド       | 説明                                                                                       |
| -------------- | ------------------------------------------------------------------------------------------ |
| `make`         | `make all` と同じく、すべてのソースファイル (`main.c`, `lexer.c`, `parser.c`) をコンパイルし、最終的な実行ファイル `main` を作成します。 |
| `make all`     | すべてのソースファイル (`main.c`, `lexer.c`, `parser.c`) をコンパイルし、最終的な実行ファイル `main` を作成します。 |
| `make clean`   | 生成されたオブジェクトファイル (`main.o`, `lexer.o`, `parser.o`) および実行ファイル `main` を削除します。    |
| `make distclean` | `make clean`を実行し、さらにプロジェクトに特有のクリーンアップを行います。                               |

## 実行方法
*※gccにてC言語のコンパイルを行える環境必須*  
プロジェクトディレクトリにて下記を実行  
```
$ make  
$ ./main ファイル名  
$ make clean  
```


## 概要
- file名は任意(上記ファイル名に拡張子も含め入力し実行)  
- 拡張子`lai`  


## 使用に関すること
- 本プログラムを使用しての事象について、一切の責任を負いかねます。
- 本プログラムを使用してのプログラムを公開する場合、このページのURLの記載をすることで使用可能です。
- 随時更新を行うため、使用する場合は確認を行うこと。


## 参考
<https://github.com/python/cpython>                      #cpythonソースコード  
~~<https://nw.tsuda.ac.jp/lec/arm64/mac_m1/>               #arm64アセンブリ~~  
~~<https://qiita.com/k0kubun/items/72d7e8c05eef14b00228>   #arm64アセンブリ~~  
<https://3iz.jp/>                                        #プログラミング言語の作り方(概要)  
<https://qiita.com/quwahara/items/09bd14ddc5de6d3f2780>  #javaでインタプリタの実装  
<https://qiita.com/sawasaka/items/2df474fc00ec2105acab>  #メモリの解放  
<https://qiita.com/Daiji256/items/09fa042e94907a232c57>  #C言語でeval風関数の実装  
<https://codechord.com/2012/01/readme-markdown/>         #README.mdファイル。マークダウン記法まとめ