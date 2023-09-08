# WebServer-with-stackchan (WSS)


## Features（特徴）
<b>(1).ファイル管理つき、WebServer</b><br>
SPIFFS/SDにPCから直接ファイルをアップロードまたは、PCにファイルをダウンロードすることが可能になりました。

![画像](images/gazo01.png )<br>
<br>

<b>ファイル管理機能</b>
1. Files of SPIFFS/SD file system
2. Upload a file to SPIFFS/SD from PC
3. Download a file from SPIFFS/SD to PC
4. Stream a file to PC browser
5. Delete a file in SPIFFS/SD
6. Rename a file in SPIFFS/SD
7. Spiffs/Sd change file system
8. Status of m5Stack system information<br>
（※注）現在、「フォルダ」操作に対応はしてません。<br><br>


<b>(2).Webアプリとファームウエアが連携したスタックチャン・ソフトウエア。</b><br>
設定・サーボ調整・リモコンのオールインワンのソフトを提供。<br>
スマートフォンとPC両方に対応しています。<br>
Webアプリは、html、styleSheet、javaScript、画像ファイルなどを使用して作成できます。<br>
(　↓　スタックチャン用のリモコン　)

![画像](images/s-remote-smaho.png)<br>
<br>

<b>(3).５階層構造</b><br>
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

## LICENCE
作成したソースファイルは、ファイル名の先頭に "ws" を付けています。<br>
本ソフトウエアは、主に下記のソフトを参考にして作成しました。<br>
・ AI_StackChan2 (robo8080さん)<br>
・ stack-chan-tester(タカオさん)<br>
・ ESP-File-Server（David Bird:  UK）<br>
<br>
本ソフトのライセンスは、MITライセンスです。<br>
AI_StackChan2、stack-chan-tester と同じライセンスです。<br>
<br>
ただし、ESP-File-Server（David Bird: UK）は、個別ライセンスを指定しています。<br>
GitHubの "doc/ESP-File-Server-Licence.txt" を読んで、ご自分の責任でご利用ください。<br>
（ 該当ソース・ファイルは、"src/base00/wsServer.cpp" です。）<br>
<br>

## Ver3.01 2023-08-23　
first Release
<br><br>