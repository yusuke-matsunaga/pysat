.. _SatBool3:

SatBool3 クラス
===============

SAT ソルバ上のブール値を表すクラス．
通常ブール値は `True` と `False` の２値だが，
ここでは不定を表す X を含めて3値としている．

.. table::
   :align: left

   ============== ========== =========================
   値             文字列表現 意味
   ============== ========== =========================
   SatBool3.true  "true"     真
   SatBool3.false "false"    偽
   SatBool3.x     "x"        不定
   ============== ========== =========================

SatBool3 の演算
---------------

以下の例の `v1`, `v2`, `v3`
はそれぞれ `SatBool3` 型のオブジェクトとする．

.. code-block:: python

   v2 = ~v1

反対の値を返す．
ただし， `SatBool3.x` に対しては `SatBool3.x` を返す．

::

   v3 = v1 * v2

ブール代数的には排他的論理和の演算を行う．真理値表は以下の通り．

.. table::
   :align: left

   ======= ======= ======= =======
   `*`     "true"  "false" "x"
   ======= ======= ======= =======
   "true"  "true"  "false" "x"
   "false" "false" "true"  "x"
   "x"     "x"     "x"     "x"
   ======= ======= ======= =======


::

   v1 *= v2

上記の演算の自己代入形．


::

   v1 == v2

等価比較演算を行う． `v1` と `v2` が等しい時 `True` を返す．
`x` と `x` も等しいとみなす．
演算結果は `SatBool3` ではなく Python のブール値であることに注意

::

   v1 != v2

非等価比較演算を行う． `v1` と `v2` が等しくない時 `True` を返す．
演算結果は `SatBool3` ではなく Python のブール値であることに注意


`SatBool3` のオブジェクト生成
------------------------------

::

   v_true = SatBool3("true")
   v_true == SatBool3.true

   v_false = SatBool3("false")
   v_false == SatBool3.false

   v_x = SatBool3("x")
   v_x == SatBool3.x

この様に文字列から `SatBool3` のオブジェクトを生成することができる．
実はこの文字列は大文字小文字を区別しないので `SatBool3("TRUE")`
でも `SatBool3.true` を生成する．
