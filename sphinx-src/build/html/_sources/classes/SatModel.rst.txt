.. _SatModel:

SatModel クラス
===============

SAT の解を表すクラス．
具体的には各変数に対する真偽の割り当てを表す．
通常は `SatSolver.solve()` の結果として生成される．
ユーザーが 'SatModel' のインスタンスを生成することはできない．


SatModel の演算
---------------

以下の例では `model` は SatModel のインスタンス，
`l` は `SatLiteral` (:ref:`SatLiteral`) のインスタンスとする．

要素数の取得
^^^^^^^^^^^^

.. code-block:: python

   n = len(model)

で `model` の要素数を得る．
ただし，この値を使うことはほとんどない．

値の取得
^^^^^^^^

.. code-block:: python

   v = model[l]

で `model` の `l` に対する値( `SatBool3` (:ref:`SatBool3`))を返す．
仕様上，値は `SatBool3.true` `SatBool3.false`
`SatBool3.x` の３通りありうるが，実際には
`SatBool3.true` か `SatBool3.false`
の２つしかない．

変数 `x` の肯定リテラルを `plit`，否定リテラルを `nlit`
とすると `model[plit] = ~model[nlit]` が常に成立する．
