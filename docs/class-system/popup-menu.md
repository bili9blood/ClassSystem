# 悬停菜单

悬停在屏幕右侧，提供了一些可交互的工具，点击展开/收起。

展开后的功能有：[截图](#截图)、[表格](#表格)、[帮助](#帮助) 。

<div class="inline-img">

![收起状态的悬停菜单](/img/popup-menu.png)

![展开状态的悬停菜单](/img/popup-menu-opened.png)

</div>

## 截图

点击唤起截图功能并打开截图界面。

![截图界面](/img/print-screen.png)

| 操作                | 功能     |
| ------------------- | -------- |
| <kbd>Esc</kbd>      | 退出     |
| <kbd>鼠标双击</kbd> | 退出     |
| 拖拽鼠标画出矩形    | 矩形截图 |

按住鼠标左键拖拽鼠标进行矩形截图，选中截图位置后可点击 `确认` 按钮完成截图或点击 `取消` 按钮退出截图。若要重新选取截图区域直接再次拖出矩形即可。

完成截图后，会自动将截图保存在 `程序路径/screenshots` ，同时在桌面生成一个 [贴图](#贴图)。

### 贴图

将图片固定在桌面。

::: tip
贴图只会在完成截图后自动开启
:::

单击贴图使其获取[焦点](//baike.baidu.com/item/焦点/10757381)后，有以下操作：

<style>
    .first-col {
        text-align: left;
    }
    .first-col > kbd {
        font-weight:normal;
    }
</style>

<table>
    <thead>
        <tr>
            <th>按键</th>
            <th>功能</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <th class="first-col"><kbd>BackSpace</kbd></th>
            <td rowspan=4> 关闭贴图（不可再次开启）</td>
        </tr>
        <tr>
            <th class="first-col"><kbd>Delete</kbd></th>
        </tr>
        <tr>
            <th class="first-col"><kbd>鼠标中键</kbd></th>
        </tr>
        <tr>
            <th class="first-col"><kbd>鼠标双击</kbd></th>
        </tr>
        <tr>
            <th class="first-col"><kbd>鼠标滚轮</kbd></th>
            <td>缩放贴图（ 50%~300% ）</td>
        </tr>
    </tbody>
</table>

## 表格

显示完整的 [课程表](/class-data#课程)、[抬饭表](/class-data#抬饭生)、[值日表](/class-data#值日生) ，表中当日列高亮。

![表格](/img/table-window.png)

鼠标点击上方选项卡以切换表格，在表格上滚动鼠标滚轮以调整字体大小。

## 帮助

用默认浏览器打开 [ClassSystem 使用文档](/) 。
