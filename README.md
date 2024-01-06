# WebServer-with-stackchan (WSS)

## New（Ver304 新機能）
<b>(1).ステータス表示</b><br>
バッテリー・アイコンを拡張してステータス表示ができるよになりました。<br>
画面上部をタッチすると、６種のモード切替え・１０秒表示・常時表示OnOff機能を実行します。

![画像](images/statusLineMode.jpg)<br>
<br>

<b>(2).イラストのHome画面表示（自由に選択可）</b><br>
おきもくさんがまとめてくれた、自由に使えるイラストをHome画面に表示できるようになりました。現在、ご提供いただいたイラスト９＋１(下の画像)の計１０種類のファイルが選択できます。
![画像](images/home-gazo03.png)<br>
<br>
<br>

<b>(3).スタックチャン用リモコンの更新（Remote StackChan）</b><br>
ステータス表示の対応とKeyLock・Rebootボタンが追加されました。

![画像](images/remoteStackChan.png)<br>

<br>
<br>

## Features（特徴）
<b>(1).ファイル管理機能つき、WebServer</b><br>
SPIFFS/SDにPCから直接ファイルをアップロードまたは、PCにファイルをダウンロードすることが可能になりました。


![画像](images/gazo01.png )<br>
<br>

<b>ファイル管理機能</b><br>
Dir of SPIFFS/SD file system<br>
Upload a file to SPIFFS/SD from PC <br>
Download a file from SPIFFS/SD to PC <br>
Stream a file to PC browser<br>
Delete a file in SPIFFS/SD<br>
Rename a file in SPIFFS/SD<br>
Spiffs/Sd change file system<br>
Status　of m5Stack system information<br>

<b>SD folder function (暫定版)</b><br>
Root  : change directory to Root<br>
Chdir. : change directory<br>
Mkdir : make directory<br>
Rmdir: remove directory<br>
<br>

<b>(2).Webアプリとファームウエアが連携したスタックチャン・ソフトウエア。</b><br>
設定・サーボ調整・リモコンのオールインワンのソフトを提供。<br>
スマートフォンとPC両方に対応しています。<br>
Webアプリは、html、styleSheet、javaScript、画像ファイルなどを使用して作成できます。<br>
<br>

<b>(3).階層構造</b><br>
ソフトウエアを開発するにあたり、開発効率、拡張性とメンテナンスを考慮した設計をしました。<br>
各階の役割は次のようになっています。<br>
４階： ｗebApps: リモコン、Servo調整、設定などのWebアプリ（SPIFFS）<br>
３階： webApi: 外部とのインターフェース<br>
２階： module: ChatGPT,Timer,Servoなど、ファームウエアで提供のモジュール<br>
１階： base1: Avatar,TTS,STTなど、スタックチャンの基となる表情・話す・聞くなどの部分<br>
０階： base0: M5Stackのハード依存の部分(Button,LED,Server,WiFiなど)<br>
<br>

<b>(4).module(モジュール)形式で、機能の追加・削除が可能。</b><br>
現在ファームウエアとして３個のモジュールの内蔵アプリを提供。<br>
・　chatGPT　（AiStackchan２相当）<br>
・　Timer<br>
・　Servo<br>
<br>


## 必要なもの
・　本体： M5Stack Core2  ( Core2 for Awsで動作確認しています。）<br>
・　SDカード （初回のWiFi接続時のみ使用）<br>
・　PC ＋ VsCode開発環境：ファームウエアをM5StackCore2に書き込めること<br>
・　Servo（足、胴体部分： 「顔だけ」の場合は必要なし）<br>
・　OpenAiのApiKey<br>
・　VOICEVOXのApiKey<br>
<br>


## Support Page
下記リンクで、インストール手順などのサポートを行っています。<br>
https://nori.rdy.jp/wss/   　　　(WSS-Support)<br>


![画像](images/s-support-page.png)<br>
<br>

## M5Burner
M5Burnerで、ファームウエアを入手することもできます。<br>
（デバイス種類は「CORE2 & TOUGH」）<br>
![画像](images/s-M5Burner03.png)<br>
<br>

## LICENSE

私が作成した部分のソフト(srcフォルダ以下:libフォルダは除く)は、[MIT license](LICENSE) です。<br>
ただし、"src/base00/wsServer.cpp"ファイルは、[ESP-File-Server-License](doc/ESP-File-Server-License.txt)　となります。<br>
ご自分の判断でご活用ください。<br>

イラストのライセンスは下記をご参照ください。<br>
  - [おきもくさんの公開物wiki] (https://okimoku.com/wiki/%E3%82%A4%E3%83%A9%E3%82%B9%E3%83%88)

<br>


## 参考リポジトリ
- [AI_StackChan2 (robo8080さん)](https://github.com/robo8080/AI_StackChan2)「MIT License」
- [M5Stack_Stack-chan_another_dimension(つゆきぱぱさん)](https://github.com/yakumo0623/M5Stack_Stack-chan_another_dimension)「MIT License」 
- [stack-chan-tester (タカオさん)](https://github.com/mongonta0716/stack-chan-tester)「MIT License」

- [m5stack-avatar(ししかわさん)](https://github.com/meganetaaan/m5stack-avatar)「MIT License」

- [ESP-File-Server(David Bird:UK)](https://github.com/G6EJD/G6EJD-ESP-File-Server)： [ESP-File-Server-License](doc/ESP-File-Server-License.txt)
<br><br>


## 「スタックチャン」について
「スタックチャン」は、[ししかわさん](https://github.com/meganetaaan)が公開しているオープンソースのプロジェクトです。<br>
https://github.com/meganetaaan/stack-chan
<br>
<br>



## 謝辞
このソフトを作成するにあたり、<br>

- AiStackChan2の開発者で、先進的な取り組みでスタックチャン界をリードしてくださる[robo8080](https://github.com/robo8080)氏に感謝いたします。<br>

- stack-chan-testerの開発者で、博識でいつも適切なアドバイスをして頂ける[タカオ](https://github.com/mongonta0716)氏に感謝いたします。<br>

- スタックチャン・コミュニティを様々な角度から暖かく支えてくれて、自由に使えるイラストをまとめてくれた[おきもく](http://okimoku.com/gallery/)氏に感謝いたします。<br>

他にも、たくさんの方々からアドバイスとアイデアを頂き、感謝いたしております。
<br><br>

## Author
[NoRi](https://twitter.com/NoRi230401)
<br>

## history
### Ver3.04 2024-01-06
　
### Ver3.03 2023-12-24　
・CORE2 V1.1対応 -- M5UnifiedライブラリV0.1.11使用
### Ver3.02 2023-12-22　
<b>(1).　Chat（Chat Remote Controller）Webアプリ</b><br>
ChatGPTの操作に特化したリモコンを追加しました。<br>
・　キャラ変機能<br>
９つのキャラクター（ロールと音声）をワンタッチで変更<br>
・　チャット履歴表示（chat History）<br>

![画像](images/chatRemoCon.png )<br>

<b>(2).　暫定版：ＳＤファイル管理のフォルダ機能</b><br>
Spiffs/SDで、ＳＤを選択するとフォルダ機能のメニューが出ます。<br>
Chdir, Mkdir, Rmdir　などが使えるようになりました。<br>
<br>

### Ver3.01 2023-08-23　
・first Release
<br>

