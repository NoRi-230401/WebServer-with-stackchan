'use strict';

  function disp(url) {
    window.open(url, "StackChanSub", "width=300,height=200");
  }

  function MovePowerOffCheck() {
    if (confirm("終了してもいいですか？")) {
      window.location.href = "http://stackchan/shutdown";
    }
    else {
      alert("「終了」をやめました。");
    }
  }

  function MoveRebootCheck() {
    if (confirm("再起動してもいいですか？")) {
      window.location.href = "http://stackchan/shutdown?reboot=on";
    }
    else {
      alert("「再起動」をやめました。");
    }
  }

// document.write("Hello World");
// console.log("hello3");

