# chineseocr_lite_dll
超轻量级图像中文识别组件 [chineseocr_lite](https://github.com/DayBreak-u/chineseocr_lite) 的 DLL 版本，用于 [aardio](http://www.aardio.com) 扩展库，适用于任何支持DLL的语言。

感谢 [xuncv/chineseocrlite-aardio](https://github.com/xuncv/chineseocrlite-aardio) 提供的 chineseocrlite-aardio 扩展库，让我了解到了  [chineseocr_lite](https://github.com/DayBreak-u/chineseocr_lite) 这个优秀的开源组件。但是我在使用 chineseocrlite-aardio 时遇到一些问题，没有找到 DLL 的源代码，翻了一下 chineseocr_lite 的源码也只有 EXE 的项目，所以花了一点时间改造了一个 DLL 版本并开源，DLL 主要用于我重新实现的 aardio 扩展库：string.ocrLite，当然也可以用于任何支持 DLL 的语言。

此 DLL使用 Unicode 路径加载文件( 可解决中文路径下崩溃的问题 )，并使用内存数据输入输出图像 —— 这样在调用DLL时可以更灵活一些。DLL 工程已经全部配置好，只要使用 VC 2017 就可以直接编译出DLL文件。也可以在 aardio 中运行 `import string.ocrLite` 即可自动下载安装好我生成好的DLL文件以及 aardio 扩展库源码，DLL 接口用法请参考 aardio 扩展库源码。

**string.ocrLite 支持生成绿色、独立的 EXE 文件**，带模型生成的EXE打包后仅 **十几** MB。下面主要介绍一下在 aaardio 中的用法，详细的用法请查看 string.ocrLite 函数库文档，以下是一个简单的例子：

```javascript
import win.ui;
/*DSG{{*/
var winform = win.form(text="string.ocrLite 简单演示";right=796;bottom=504)
winform.add(
button={cls="button";text="识别剪贴板图像";left=528;top=423;right=712;bottom=477;db=1;dr=1;z=2};
edit={cls="edit";left=497;top=36;right=764;bottom=403;db=1;dr=1;dt=1;edge=1;hscroll=1;multiline=1;vscroll=1;z=3};
plus={cls="plus";left=25;top=36;right=474;bottom=403;db=1;dl=1;dr=1;dt=1;repeat="scale";z=1}
)
/*}}*/

import string.ocrLite;
import string.ocrLite.defaultModels;
import win.clip;

var ocr = string.ocrLite(,true);
winform.button.oncommand = function(id,event){
	var hBmp = win.clip.readBitmap()
	if(!hBmp){
		return;
	}
	
	var bmp = gdip.bitmap(hBmp);
	var ocrRet = ocr.detectBitmap(bmp);
	if(ocrRet){
		winform.plus.background = ocrRet.resultBitmap;
		
		winform.edit.text = ocrRet.text;
		winform.edit.text = "";
		
		import web.json;
		for i,block in table.eachIndex(ocrRet.blocks){
			winform.edit.print(block.text);
			winform.edit.print(web.json.stringify(block.points,false));
		}
	}
}

winform.show();
win.loopMessage();
```

运行效果如下：
![OCR 示例效果 ](https://github.com/aardio/chineseocr_lite_dll/raw/main/screenshots/ocr.gif)