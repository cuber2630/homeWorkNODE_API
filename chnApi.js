var addon = require('bindings')('chnApi');


console.log('test napi.h start');

var obj = new addon.ChnApi(10);
console.log( obj.plusOne() ); // 11
console.log( obj.multiply().value ); // 11
console.log( obj.multiply(10).value ); // 110
var newobj = obj.multiply(-1);
console.log( newobj.value ); // -11
console.log( obj === newobj ); // false

console.log( 'get math' ); // false
console.log( newobj.getGameBase() );//11
console.log( obj.getGameBase() );//11

//抓API版本
console.log( obj.getApiVersion() );//10
//新玩家，傳入 新BUF資料給機率。此時會抓到機率版本
console.log( obj.initNewUser() );//10
//抓機率版本
console.log(obj.getMathVersion());//10



let a = obj.jsonExample();
console.log( a );//1
a.js = 0;
console.log( a );//1

//抓系統設定給
let loginData = {
    gameData: {
        a: 1,
        b: 2,
        c: 3
    },
    userId: 8825252,
    userSeat: 2,
    rtp: 9500,
    demo: 1,
    otherPool: 0,
    line: 1,
    betFromLength: 12,
    betFrom: [1, 2, 3, 4, 5, 10, 20, 50, 100, 200, 500, 1000],
};

console.log( obj.setUserLogInData(loginData) );//10







