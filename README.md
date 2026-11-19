![Jv Song · Software QA / IoT](https://raw.githubusercontent.com/Jvsong/Jvsong/main/profile/running-track.svg)

你好，我是 **Jv Song**。我做软件测试，平时和手机 App、接口、物联网设备打交道。测定位器时，我喜欢带着设备和运动手表走同一条路线，回来看两条轨迹在哪个弯道开始分开。

工作之外我喜欢长跑，也在用 Python 和 Java 把自己的想法做成工具。

## 三个值得点开的项目

| 项目 | 打开后可以看什么 |
| --- | --- |
| [电商后台自动化测试](https://github.com/Jvsong/graduation-project) | 从登录冒烟测试开始，看一条用例如何执行，以及失败后去哪里找报告和截图。 |
| [Runalyzer](https://github.com/Jvsong/Runalyzer---) | 把 Garmin 的 FIT 或其他运动记录导进去，对照配速、心率和海拔，看看哪一段跑得最吃力。 |
| [ShopPilot Admin](https://github.com/Jvsong/-ShopPilot-Admin-) | 一个用 Spring Boot 和 Vue 3 写的电商后台，可以沿着商品、订单和角色权限读前后端代码。 |

## 🔎 玩一局：消失的定位点

你在公园测一台定位器。刚走到东门，地图上的图标却跳回了西门。下面是手机收到的两条模拟记录，按**到达手机的顺序**排列。

| 到达顺序 | 设备定位时间 | 位置 |
| :---: | :---: | :---: |
| ① | 09:42:18 | 东门 |
| ② | 09:40:06 | 西门 |

**如果你来修这个问题，会先试哪一招？** 点选一项揭晓结果。

<details>
<summary>A · 总是把最后收到的点画在地图上</summary>

再看一眼两只钟。第二条消息确实来得晚，可它记录的位置比第一条早了两分多钟。试试另一个选项。
</details>

<details>
<summary>B · 用设备定位时间判断哪个点更新</summary>

🎯 找到了！本例中，同一设备的两条记录使用同一时钟。西门的旧点可以补进历史轨迹，实时标记应停在东门。

**加一道回归用例**：先送达东门的新点，再送达西门的旧点。检查实时标记仍在东门，打开历史轨迹则能按定位时间看到「西门 → 东门」。
</details>

<details>
<summary>C · 把地图刷新频率提高一倍</summary>

刷新得更勤，旧点依然可能最后到达。试着找一列能告诉你「定位器当时在哪里」的数据。
</details>
