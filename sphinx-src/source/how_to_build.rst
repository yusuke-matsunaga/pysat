pysat の取得，ビルド，インストール
===================================

レポジトリの取得
^^^^^^^^^^^^^^^^

.. code-block:: shell

   $ git clone --recurse-submodule https://github.com/yusuke-matsunaga/pysat.git

でソース一式を取得する(`$` はコマンドプロンプト．以下同様)．
`--recurse-submodule` オプションを付けなかった場合には clone ごリポジ
トリに移動して以下のコマンドを実行する．

.. code-block:: shell

   $ git submodule init
   $ git submodule update


ビルドディレクトリの作成
^^^^^^^^^^^^^^^^^^^^^^^^^

ソースディレクトリの中にビルドディレクトリを作ることもできるが，
git で管理されたリポジトリ内に作業用ファイルを作るのは美しくないので
別にビルドディレクトリを用意する．
このビルドディレクトリを ${BUILD_DIR} と表すことにする．
ソースディレクトリ直下で以下の操作を行う．

.. code-block:: shell

   $ ./mk_builddir.sh ${BUILD_DIR}

${BUILD_DIR} が存在しない場合には新規に作成する．
実行してよいか尋ねるプロンプトが出るので `yes` か `no` を選択する．

既存のディレクトリを再利用する場合はすでにある設定ファイルを置き換えて
よいかを尋ねるプロンプトが出るので同様に `yes` か `no` を選択する．
結果として ${BUILD_DIR} の下に `boot.sh` と `boot-opt.sh` という2つの
ファイルが生成される．
逆に言うと `mk_builddir.sh` の仕事は ${BUILD_DIR} を作成してこの2つの
ファイルをコピーすることである．

ビルド環境の設定
^^^^^^^^^^^^^^^^

${BUILD_DIR} に移動してビルドシステム用の設定を行う．
具体的には以下の操作を行う．

.. code-block:: shell

   $cd ${BUILD_DIR}
   $./boot.sh

すると内部で `cmake` を起動して必要な処理を行う．
なお，いくつか設定可能がオプションがある．
また，`bison` のようなサードパーティ製のソフトウェアを使う場合に
パス指定が必要な場合がある．
これらの設定はそのサイトにローカルな設定なので `boot.sh` は変更せずに
`boot-opt.sh` に記述しておく.
`boot.sh` は実行時にに `boot-opt.sh` ファイルの内容を読み込んで設定に
反映させる仕組みになっている．

具体的には `cmake` へ与えるオプションは `options` というシェル変数に記
述する．
複数のオプションを記述することを考えて以下のような形式にしておくと
管理がやりやすい．

.. code-block:: shell

   options="$options -DXXX=YYY"

この例では `cmake` が用いる `CMakeLists.txt` 中の変数 `XXX` に値 `YYY`
を割り当てる操作を行っている．

環境変数は通常のシェルスクリプト同様に設定する．

.. code-block:: shell

   export PATH="/usr/local/opt/bison/bin:$PATH"

この例では実行パスの先頭に `/usr/local/opt/bison/bin` を追加している．
実際，Mac の場合，デフォルトで入っている yacc(bison) のバージョンが
古いのでポートで最新の bison をインストールする必要があり，
このような記述が必要となる．

ビルド
^^^^^^^

`ninja` が使用可能なら `ninja` をそうでないなら `make` を用いてビルド
を行う．
どのビルドシステムが使用されているかは `boot.sh` 実行時のメッセージを
確認すればよい．

インストール
^^^^^^^^^^^^^

ビルドが終了したら `ninja install` もしくは `make install` でビルド結
果のインストールが行われる．
とは言ってもユーザ権限でシステム領域にファイルを書き込むことはできない
ので，
現在は ${BUILD_DIR}/stage というディレクトリの下にファイルをコピーして
いるだけである．
具体的には以下のファイルが生成される(unixの場合)．

.. code-block:: shell

   ${BUILD_DIR}/stage/bin/pysat
		     /lib/python{version}/site-packages/ymsat.so

`python{version}` は実際には使用している python のバージョンが入る．
(例えば python3.9 など)．

このうち，`ymsat.so` は動的ライブラリで Python の拡張モジュールとなっ
ている．
`pysat` はあらかじめ `ymsat.so` を組み込んだ python インタプリタである．
通常の `python3` と同様に使うことができる．
