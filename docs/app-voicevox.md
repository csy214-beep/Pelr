### voicevox 配置指引

```markdown
VOICEVOX 软件使用条款

### 許諾内容

- 它可以用于商业和非商业用途
- 使用创建的音频时，请遵守各音频库的使用条款
- 当你将创作音频的使用授权给他人时，你必须要求对方遵守本许可证第2和第3条

### 禁止事項

- 未经许可，重新分发全部或部分软件。
- 反编译、逆向工程以及公开披露这些方法
- 对创作者或第三方造成不利的行为
- 破坏公共秩序和道德的行为

### 免責事項

软件开发者不对软件造成的任何损害或不利行为承担责任。

### 其他

使用VOICEVOX时，您必须有一份显示您已使用VOICEVOX的署名。
```

注意，每个模型有其相应的利用条款，请自行查看。

本程序的日语TTS模块基于`voicevox_core c_api 0.16.4`开发

[voicevox_core](https://github.com/VOICEVOX/voicevox_core)
官网：<https://voicevox.hiroshiba.jp/>

本程序不提供voicevox_core的`辞书`和`模型`文件，需要用户按需下载。

简明配置：

前往：<https://github.com/VOICEVOX/voicevox_core/releases/tag/0.16.4>

本程序使用的是`0.16.4`版本的c_api

下载：`download-windows-x64.exe`

运行该程序后，将下载后的文件夹（约1G）放到`Resources`目录下

模型选择：https://github.com/VOICEVOX/voicevox_vvm

模型预览可前往官网。

或者自定义，但是要在设置中进行相应配置，不论是否将文件放到推荐目录。

设置页上如果未显示语音风格但已进行配置属于正常现象，依然可以测试成功。

典型配置：

<details>
<summary>预览</summary>

![alt text](assets/image-19.png)

</details>
