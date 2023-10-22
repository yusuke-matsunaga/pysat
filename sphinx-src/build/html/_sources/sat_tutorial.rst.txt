.. _sat_tutorial:

pysat の使い方
===============

pysat とは
----------

pysat は `python` から SAT ソルバを使えるようにするための拡張モジュールである．

具体的には `ymsat` という名前の動的ライブラリを提供している．
`python` のモジュール検索パスに含まれる適切なディレクトリにこの動的ラ
イブラリを置くことによって `import ymsat` で `ymsat` が使用可能となる．
また，あらかじめ `ymsat` を組み込んだ `python` インタプリタとして
`pysat` というバイナリプログラムも提供している．
こちらは動的なモジュール検索パスの設定は必要なく， `import ymsat`
を実行することができる．

以下の説明は通常の `python` に動的に `ymsat` モジュールを読み込んだ
場合と `pysat` を実行した場合のどちらでも同一の結果を得ることができる．
なお，プログラムの先頭に以下の記述があるものとする．

.. code-block:: python

   from ymsat import SatBool3, SatSolver


簡単な SAT 問題を解く
-----------------------

ソルバの生成
^^^^^^^^^^^^^

ここで， :math:`(\neg v_1 \vee \neg v_2) \wedge (v_2 \vee \neg v_3) \wedge (v_1 \vee v_3)`
というSAT問題を解くことを考える．
そのためにはまず SATソルバのインスタンスを生成する必要がある．

.. code-block:: python

   solver = SatSolver()

`solver` は `SatSolver` 型(:ref:`SatSolver`)のオブジェクトである．

変数の割り当て
^^^^^^^^^^^^^^^

次に論理式を記述するための変数の割り当てを行う．
変数は内部ではただの整数で表されるが，
SATソルバの内部で必要な処理があるため，
適当な数字を勝手に使うことはできない．
そこで以下のように変数を割り当てる．

.. code-block:: python

   v1 = solver.new_variable()
   v2 = solver.new_variable()
   v3 = solver.new_variable()

`SatSolver.new_variable()` は `SatLiteral` 型(:ref:`SatLiteral`)のオブジェクトを返す．
これは変数そのものではなく，変数の肯定を表すリテラルであることに注意．
変数そのものを表すオブジェクトは存在しない．
なお，`v1` などの `python` 上の変数名は SATソルバに対してはなんの意味
も持たない． `v1` の代わりに `x` を用いても結果は全く同じである．

問題(論理式)の作成
^^^^^^^^^^^^^^^^^^^^

一般に SATソルバは問題を内部に保持しておく形式をとる場合が多い．
これは問題に対する解析情報も内部で保持しておくことで効率的な
処理を行うためである．
そのため，SAT問題という別の型のオブジェクトは存在せず，
SATソルバに対して問題(の部品であるCNF節)を追加していくことになる．

前述の論理式を追加するプログラムは以下のようになる．

.. code-block:: python

   solver.add_clause(~v1, ~v2)
   solver.add_clause(v2, ~v3)
   solver.add_clause(v1, v3)

`SatSolver.add_clause()` の詳細な使い方に関しては :ref:`SatSolver` を
参照せよ．

SAT問題を解く
^^^^^^^^^^^^^^

問題を設定したら最後に解を求める．

.. code-block:: python

   ans = solver.solve()

この問題は充足可能なので `ans = SatBool3.true` となる．
結果が充足可能だった場合，充足解の一つを取り出すことができる．

.. code-block:: python

   model = solver.model()

`model` は `SatModel` 型(:ref:`SatModel`)のオブジェクトである．
`v1, v2, v3` に対する値の割り当て結果は以下のように取り出すことができる．

.. code-block:: python

   ans_v1 = model[v1]
   ans_v2 = model[v2]
   ans_v3 = model[v3]

なお，`SatSolver.model()` では全ての変数に対する割り当て結果をコピーす
るので一部の変数割り当ての結果だけを調べる場合には効率的ではない．
その場合は `SatSolver.read_model()` を用いる．
以下のコードは上のコードと同一の結果を得る．

.. code-block:: python

   ans_v1 = solver.read_model(v1)
   ans_v2 = solver.read_model(v2)
   ans_v3 = solver.read_model(v3)

ただし，`SatSolver.solve()` が呼ばれると以前の結果はクリアされるので
注意が必要である．
過去の割り当て結果が必要なら `SatSolver.model()` を使って結果をコピー
する必要がある．

SAT問題を解く際に一部(もしくは全部)の変数の値を固定することができる．
このような値の割り当てを assumptions と呼ぶ．
assumptions を与えるには `SatSolver.solve()` の引数に
`SatLiteral` のリストを与えればよい．

.. code-block:: python

   ans2 = solver.solve([v1])

この例では上と同じ問題に対して :math:`v_1 = true` となる割り当て
を与えて解を求めている．

別の問題を解く
^^^^^^^^^^^^^^^^

前述の通りSATソルバは内部で現在解いている問題の解析情報を保持している
ため，
別の問題を解く場合には別の `SatSolver` オブジェクトを生成する必要があ
る．
ただし，前述の通り，同じ問題に対して assumptions を変更して解く場合は
同一のSATソルバを使うことができる．
また，現在の問題に対して新たな節(論理式)を追加して新しい問題にする場合
も同一のSATソルバを使うことができる．
ただし，一度追加した節を削除することはできない．
