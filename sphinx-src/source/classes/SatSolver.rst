.. _SatSolver:

SatSolver クラス
================

SAT ソルバを表すクラス．
一般に SAT ソルバは内部に現在実行中の SAT 問題を表す CNF とそれに付随
した情報を持つ．
そのため，一つのソルバを使って異なる複数の問題を解くことはできない．
ただし，現在の問題に対して節を追加することはできる．
既に追加した節をあとから削除することはできない．


SatSolver の生成
----------------

生成方法にはいくつかのパタンがある．

.. code-block:: python

   solver = SatSolver()

デフォルトタイプのソルバを生成する．
デフォルトタイプの指定方法は少し複雑なので後述する．

.. code-block:: python

   solver = SatSolver('ymsat2')

ソルバの種類を表す文字列を指定したパタン．
現在使用可能はタイプは以下の通り．

.. table::
   :align: left

   ============ =====================
   文字列       説明
   ============ =====================
   minisat      MiniSat
   minisat2     MiniSat2
   glueminisat2 glueminisat2
   lingeling    lingeling
   ymsat2       自作のSATソルバ
   ============ =====================

.. code-block:: python

   init_param = {
     type: 'ymsat2',
     controller: 'minisat2',
     analyzer: 'uip2',
     selector: {
       type: 'nega',
       phase_cache: true
     }l
   }

   solver = SatSolver(init_param)

初期化パラメータを表す辞書(dict)を引数として与えるパタン．
細かな設定を行うことができる．
使用可能な項目はソルバのタイプごとに異なる．


デフォルトタイプの詳細
^^^^^^^^^^^^^^^^^^^^^^

引数を与えずに `SatSolver` を生成する場合，
以下のアルゴリズムで初期化パラメータの設定を行う．

1. 環境変数 `YMSAT_CONF` を参照する．
   `YMSAT_CONF` が設定されている場合，
   その値を設定用の JSON ファイルとみなして
   読み込む．読み込んだ結果の辞書を引数として
   `SatSolver` を生成する．

   読み込みが失敗した場合や環境変数が未設定の場合，
   次へスキップする．

2. 環境変数 `YMSAT_CONFDIR` を参照する．
   `YMSAT_CONFDIR` が設定されている場合，
   ${YMSAT_CONFDIR}/ymsat.json を設定用JSONファイルと
   みなして読み込む．

   読み込みが失敗した場合や環境変数が未設定の場合，
   次へスキップする．

3. カレントディレクトリの ymsat.json ファイルを読み込む．

   読み込んだ結果を初期化パラメータとして用いる．

   読み込みが失敗した場合やファイルが存在しない場合は
   次へスキップする．

4. ハードコードされたデフォルト値を用いる．

   現時点では { type: 'ymsat2'} を用いる．


変数の生成
-----------

.. code-block:: python

   v1 = solver.new_variable()
   v2 = solver.new_variable(False)

論理式中で用いる変数を生成する．
実際に返されるのはその変数の肯定のリテラルを表す
`SatLiteral` (:ref:`SatLiteral`) のインスタンスである．
変数そのものを表すクラスは存在しない．
変数は decision variable かどうかのフラグを持つ．
引数としてブール値を与えた場合にはその値が decision variable
フラグの値となる．
引数を省略した場合のデフォルト値は `True` である．

decision variable の指定がない変数はソルバ内部で最適化が行われる際に
削除される可能性がある．
そのため結果のモデル(`SatModel` (:ref:`SatModel`))から値を取り出すことができない．

ただし，ソルバタイプによってはこの decision variable の指定が意味を
持たない場合もある．


節の生成
----------

.. code-block:: python

   solver.add_clause(v1, ~v2, v3)
   solver.add_clause([v1, ~v2, v3])
   solver.add_clause([v1, ~v2], v3)
   solver.add_clause((v1, ~v2, v3))
   solver.add_clause(v1, (~v2, v3))

のようにリテラルのリスト(集合)を引数に与える．
上の5つの文はすべて :math:`(v_1 \vee \neg v_2 \vee v_3)`
を表す CNF 節を追加する．
このように引数解釈は柔軟にしてあり，最終的にリテラルのリスト(集合)
と考えられるものであればなんでもよい．
より詳細には 'シーケンス型' であればなんでもよい．


SAT問題を解く
--------------

.. code-block:: python

   ans = solver.solve()

で今までに追加された CNF 節の集合を一つのSAT問題とみなして
解を求める．
ans は `SatBool3` (:ref:`SatBool3`) 型のインスタンスで
`SatBool3.true` で SAT(充足可能)，
`SatBool3.false` で UNSAT(充足不能)，
`SatBool3.x` で不明を表す．
通常は `true` か `false` となる．`x` になるのは途中で
計算を打ち切った時のみ．

問題を解く際にあらかじめいくつかの変数に値を割り当てておく場合
がある．このような値の割り当てを assumptions と呼ぶ．
assumptions はリテラルのリストの形で与える．
こちらも `add_clause` 同様に柔軟に引き数の解釈を行うことが可能となって
いる．

.. code-block:: python

   ans = solver.solve((~v1, v3))
   ans = solver.solve(assumptions=[~v1, v3])

assumptions は 0 番目の省略可能位置引数であるが，
`assumptions` というキーワードでも指定可能となっている．
上の２つの文は同じ意味を持つ．
`add_clause` の場合と違って `solve(~v1, v3)` とはできないことに注意．
(assumptions は一つの引数でなければならない)．

ソルバタイプによっては計算時間に制限をもたせることができる．
制限時間はキーワード引数 `time_limit` で与える(単位は秒)．

.. code-block:: python

   ans = solver.solve(time_limit=3600)

制限時間に達して計算が打ち切られた場合，`ans` は `SatBool3.x`
となっている．

解の取得
^^^^^^^^^

SAT問題が充足可能だった場合，
充足可能な割り当ては以下の文で得ることができる．

.. code-block:: python

   model = solver.model()

`model` は `SatModel` (:ref:`SatModel`) のインスタンスである．
ただし，この関数ではソルバが持っている全ての変数に対する
割り当て結果の複製を行うため，いくつかの変数の値を調べる
場合にはあまり効率がよいとは言えない．
そこで以下のように一つ一つの変数の割り当て結果を調べることができる．

.. code-block:: python

   val1 = solver.read_model(v1)
   val2 = solver.read_model(~v2)

この例ではリテラル `v1` と `~v2` に対する値の割り当て結果をそれぞれ
`val1` と `val2` に格納している．
このように否定のリテラルに対する値も得ることができる．
(結果は肯定のリテラルの結果を否定したものとなる)．

`read_model()` は次の `solve()` が呼び出されると無効となるので注意．
以前の結果を保持しておきたい場合には `model()` を使って結果のコピーを
作成しておく必要がある．


矛盾の原因の解析
^^^^^^^^^^^^^^^^

assumptions 付きのSAT問題が充足不能だった場合，
矛盾の原因となっている assumptions の部分集合を得ることができる．

.. code-block:: python

   conf_lits = solver.conflict_literals()

`conf_lits` には `SatLiteral` のリストが格納される．


高度な使い方
-------------

`SatSolver` に与える問題は基本的にCNFの形に直して節ごとに追加する必要がある．
任意の論理式はCNFの形に変形できる(正確には充足等価なCNFに変換できる)こ
とが知られているのでこの事自体は問題ではないが，
その度にCNFに変換する処理を行うのは効率が悪い．
そこで，よく使うパタンの論理式をCNFに変換して追加する機能を
`SatSolver` に用意している．

Tseitin の符号化
^^^^^^^^^^^^^^^^^

Tseitin の符号化として知られる一般の論理式(もしくは論理ゲート)をCNFに
変換する．
以下， `x1, x2, ... y` は `SatLiteral` のオブジェクトであるものとする．

.. code-block:: python

   # y = x1
   solver.add_buffgate(y, x1)

   # y = ~x1
   solver.add_notgate(y, x1)

   # y = x1 & x2
   solver.add_andgate(y, x1, x2)

   # y = ~(x1 & x2)
   solver.add_nandgate(y, x1, x2)

   # y = x1 | x2
   solver.add_orgate(y, x1, x2)

   # y = ~(x1 | x2)
   solver.add_norgate(y, x1, x2)

   # y = x1 ^ x2
   solver.add_xorgate(y, x1, x2)

   # y = ~(x1 ^ x2)
   solver.add_xnorgate(y, x1, x2)

順に，バッファ，インバータ(NOTゲート)，ANDゲート，
NANDゲート，ORゲート，NORゲート，XORゲート，XNORゲート
の入力と出力の関係を表すCNFを追加する．
なお，最初の引数は出力のリテラルを表す．
残りの引数は上述のコードのように個別に与えてもよいし，
`SatSolver.add_clause` のようにリストの形で与えても良い．
例えば次のようなコードでも正しく動く．

.. code-block:: python

   solver.add_andgate(y, x1, [x2, x3])


加算
^^^^^^^^^

複数の変数を２進符号化されたビットベクトルとみなして加算を行う
回路の入出力の関係を表すCNFを追加する．

.. code-block:: python

   # 1ビット半加算器
   solver.add_half_adder(a, b, s, cout)

   # 1ビット全加算器
   solver.add_full_adder(a, b, cin, s, cout)

   # 加算器
   solver.add_adder(a_list, b_list, cin, s_list, cout)

`a, b, s, cin, cout` は `SatLiteral` である．
`a_list, b_list, s_lsit` は `SatLiteral` のリストで，
その要素数は一致していなければならない．
`a(a_list)` と `b(b_list)` が入力のリテラル．
`cin` がキャリー入力，`s(s_list)` が出力のリテラル，
`cout` がキャリー出力を表す．

カウント
^^^^^^^^^^

対象の変数のうち `1(True)` となっているものの数を数える回路の入力と出
力の関係を表すCNFを追加する．

.. code-block:: python

   o_lits = solver.add_counter(i_lits)

`ilits` および `o_lits` は `SatLiteral` のリストである．
それぞれ，入力のリテラル，出力のリテラルを表す．
いままでのメンバ関数と異なり出力のリテラルを引数として与えるのではない
ことに注意．
これは入力の要素数に応じて必要となる出力のビット数が異なるため，
外部でそのビット数を計算を行う手間を省くためである．
そのためこの関数内部で `SatSolver.new_variable()` が呼ばれることになる．

カウント系の条件
^^^^^^^^^^^^^^^^^

対象の変数のうち `1(True)` となっているものの数に関する条件を表すCNF
を追加する．

以下では `x1, x2, x3` は `SatLiteral` のオブジェクト，
`k` は整数とする．

.. code-block:: python

   # x1, x2, x3 のうち 1 になる物の数が高々1
   solver.add_at_most_one(x1, x2, x3)

   # x1, x2, x3 のうち 1 になる物の数が高々2
   solver.add_at_most_two(x1, x2, x3)

   # x1, x2, x3 のうち 1 になる物の数が高々k
   solver.add_at_most_k(k, x1, x2, x3)

   # x1, x2, x3 のうち 1 になる物の数が少なくとも1
   solver.add_at_least_one(x1, x2, x3)

   # x1, x2, x3 のうち 1 になる物の数が少なくとも2
   solver.add_at_least_two(x1, x2, x3)

   # x1, x2, x3 のうち 1 になる物の数が少なくともk
   solver.add_at_least_k(k, x1, x2, x3)

   # x1, x2, x3 のうち 1 になる物の数がちょうど1
   solver.add_exact_one(x1, x2, x3)

   # x1, x2, x3 のうち 1 になる物の数がちょうど2
   solver.add_exact_two(x1, x2, x3)

   # x1, x2, x3 のうち 1 になる物の数がちょうどk
   solver.add_exact_k(k, x1, x2, x3)

   # x1, x2, x3 のうち 1 になるの物の数が 1 でない．
   solver.add_not_none(x1, x2, x3)

`SatSolver.add_clause()` と同様に複数の `SatLiteral`
はリストにしてもよいし，個々の引数にしてもよい．

比較演算
^^^^^^^^^^

複数の変数を2進符号化されたビットベクタとみなして比較を行う．

`a_list, b_list` は `SatLiteral` のリストとする．

.. code-block:: python

   # a_list と b_list が等しい(=)．
   solver.add_eq(a_list, b_list)

   # a_list と b_list が等しくない(!=)．
   solver.add_ne(a_list, b_list)

   # a_list が b_list より小さい (<)
   solver.add_lt(a_list, b_list)

   # a_list が b_list より小さいか等しい (<=)
   solver.add_le(a_list, b_list)

   # a_list が b_list より大きい (>)
   solver.add_gt(a_list, b_list)

   # a_list が b_list より大きいか等しい (>=)
   solver.add_ge(a_list, b_list)

条件リテラル
^^^^^^^^^^^^^

`a` ならば `b` であるという含意はCNFでは :math:`\neg a \vee b`
となる．
含意の結果が複数リテラルの論理和の場合でも同様に
:math:`\neg a \vee b_1 \vee b_2 \vee b_3 \cdots`
となるが，論理積の場合には
:math:`(\neg a \vee b_1) \wedge (\neg a \vee b_2) \wedge (\neg a \vee b_3) \wedge \cdots`
のように退屈な式を延々と作らなければならない．
そこで，このような場合にコーディングの手間を減らす工夫として
`条件リテラル(conditional literal)` という仕組みを導入している．
`条件リテラル` はソルバ内部に保存される `SatLiteral` のリスト
であり，以下のようにセット・クリアできる．

.. code-block:: python

   # conditional literal のセット
   solver.set_conditional_literals(x1, x2, ~x3)

   ....

   # conditional literal のクリア
   solver.clear_conditional_literals()

`conditional literal` がセットされている場合には，
すべての `SatSolver.add_clause()` において，
`conditional literal` の否定のリテラルが追加される．

例えば以下のコード

.. code-block:: python

   solver.clear_conditional_literals()
   solver.set_conditional_literals(x1, ~x2)
   solver.add_clause(y1)
   solver.add_clause(~y2)

は以下のコードと同様の結果となる．

.. code-block:: python

   solver.clear_conditional_literals()
   solver.add_clause(~x1, x2, y1)
   solver.add_clause(~x1, x2, ~y2)

`SatSolver` が生成された時点では `conditional literal` は空である．
`conditional literal` は場合によっては思いがけないエラーを引き起こすの
でこまめにクリアしておいたほうがよい．
