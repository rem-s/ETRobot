# ETRobot(Docs)

### Docsブランチの概要
口頭で説明された内容をいちいちSlackに上げるのはアレなので、ここに上げることにする。

### 前書き
REMsの活動において、環境構築の仕方や技術的な問題を口頭で説明することはよくあることなのだが、口頭での説明だけだと後々思い出せないし、後々同じ質問が飛んできた時に困るので、文字としてまとめたいと思う。先輩方が口で言った内容をただ文字にうつすだけでは、人によっては分かりにくいと思うので、何も知識を持っていない方でも理解できるように書くつもりではある。そのため、上級生のような、既にある程度開発の経験のある方にとっては退屈な読み物になるかと思われる。

### 「おまじない」について

大学の講義のハンドアウトで「おまじない」という単語を目にしたことのある方がいると思う。問題の中でまだ習っていない語句や式が出てきた際、「今は、"それはそうなる決まりなんだ"と思え」というアレである。ヅ大生全員が「おまじない」を受け入れる事の出来る人間であるのなら、こんなことを書いてはいないのだが、私がSA(Student Assistant)をする中で、「おまじない」を受け入れる事が出来ず、そこで躓いて先に進むことのできない学生がちらほら見受けられた。「おまじない」に疑問を抱くのは素晴らしいことだと思う。(が、演習の時間には限りがあるので、「おまじない」の箇所を完全に理解するのは難しい。)なので、このDocsでは、極力「おまじない」を排して物事を説明しようと思う。私が言葉で説明するのが難しかったら、参考になりそうなWebsiteのURLでも貼っておく。

### ドキュメントの書き方について

どんな環境からでも見れるように、極力テキストのみで説明する。そのため、文中に特殊な記号を使うことがある。その時は、冒頭に凡例を書くこととする。

### 検証環境
|No.|Name|OS|CPU|Memory|Storage|GPU|memo|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|1|Lifebook A743|Win10Pro|i7-3540M|16GB|480GB|Intel HD4000|一般的なマシン。|
|2|Dynabook N200|Win7Str|Atom N455|2GB|128GB|Intel GMA3150|小さくてかわいい。|
|3|Inspiron 545s|Win10Edu|Xeon X3220|8GB|128GB|NVIDIA Quadro600|10年前のハイエンド。|

私は変なマシンしか持っていないので、マシン環境に起因する問題の解決には使えないと思う。しかし、開発用ツールはポンコツなPCでも動くことは証明できる。
### 執筆者について

部長(@aizu-wakamatsu)
Data Networking Laboratory, University of Aizu
