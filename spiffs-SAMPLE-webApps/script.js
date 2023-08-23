'use strict';

function disp(url) {
  window.open(url, "StackChanSub", "width=300,height=200");
}

function MovePowerOffCheck() {
  if (confirm("終了してもいいですか？")) {
    window.location.href = "http://192.168.0.100/shutdown";
  }
  else {
    alert("「終了」をやめました。");
  }
}

function MoveRebootCheck() {
  if (confirm("再起動してもいいですか？")) {
    window.location.href = "http://192.168.0.100/shutdown?reboot=on";
  }
  else {
    alert("「再起動」をやめました。");
  }
}



function set2fig(num) {
  // 桁数が1桁だったら先頭に0を加えて2桁に調整する
  var ret;
  if (num < 10) { ret = "0" + num; }
  else { ret = num; }
  return ret;
}
function showClock2() {
  var nowTime = new Date();
  var nowHour = set2fig(nowTime.getHours());
  var nowMin = set2fig(nowTime.getMinutes());
  var nowSec = set2fig(nowTime.getSeconds());
  var msg = "時刻は、" + nowHour + ":" + nowMin + ":" + nowSec + " です。";
  document.getElementById("RealtimeClockArea2").innerHTML = msg;
}
setInterval('showClock2()', 1000);

// document.write("Hello World");
// console.log("hello3");

