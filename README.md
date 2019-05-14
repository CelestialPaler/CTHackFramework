<h1>CTHackFramework 通用外挂框架</h1>

<h2>概述</h2>
    <p>
        &emsp;CTHackFramework是一个基于C++的通用外挂框架，可用于制作游戏外挂、修改器，软件破解机、注册机等程序。
    </p>
    <p>
        &emsp;该框架下包含了大量用于游戏外挂制作的类，函数与工具包，以供支持游戏外挂的快速开发。同时包含了一套外挂
        的软件架构，使该框架下的外挂具有稳定，高效的特性，同时使得外挂的扩展性与兼容性得以保证。
    </p>
        
<h2>框架结构</h2>
	<p>&emsp;框架的结构图如下。</p>
        <img src="https://github.com/CelestialTS/CTHackFramework/blob/master/res/arch.png" alt="Arch">
    <p>
        &emsp;<b>Memory</b>为内存模块，包含各类对目标进程的内存进行读取，写入，扫描等函数，负责对目标进程的内存进行操作。
    </p>
    <p>
        &emsp;<b>Process</b>为进程模块，包含对进程信息的获取与各类对进程的相关操作函数，如获取进程pid，句柄，查询
        进程加载的模块信息，进程的内存信息等等。并且包含一组封装好的进程内存数据读取函数，可以从目标进程中读取各种宽
        度的整形，浮点，布尔值等，同时也可以直接读取一段字节。反之，也可写入数据。
    </p>
    <p>
        &emsp;<b>Game</b>为游戏信息模块，主要包含两个部分<b>GameStruct</b>和<b>GameData</b>，其中<b>GameStruct</b>
        包含了关于游戏或者目标程序的相关结构体的定义，比如玩家类的结构，坐标结构，武器属性的结构等，而<b>GameData</b>包含了
        从游戏进程中获取到的各类数据以及外挂相关数据等。<b>Manager</b>通过<b>Process</b>将外挂所需的数据读取出来，并存储于<b>GameData</b>中以供
        各个功能子模块和GUI，Overlay等使用。
    </p>
    <p>
        &emsp;<b>Function</b>为功能子模块，即一种外挂功能，每个<b>Function</b>直接从<b>GameData</b>中直接获取相关数据，而数据的读取由<b>Manager</b>进行。
        同时，<b>Manager</b>也负责对于每个<b>Function</b>子功能模块进行管理，如启用停用等，并通过配置文件进行子功能模块的配置。功能子模块
        与整个程序的耦合程度较轻，方便模块的移植与修改。
        &emsp;其中，占用资源与时间较少，逻辑简单，实现软实时任务的子功能模块可以在<b>Manager</b>线程下运行，而相对重量级，执行硬实时任务，功能复杂的子模块，通常
        单独在一个新的线程中运行。这些线程也由<b>Manager</b>进行管理。
    </p>
    <p>
        &emsp;<b>Manager</b>是整个架构的核心，其负责整个架构的运转，其职能主要是：
        <ul>
            <li>通过<b>Process</b>，实现对目标进程的信息读取，内存操作等。</li>
            <li>将读取到的数据等存储进<b>GameData</b>中。</li>
            <li>负责管理<b>Function</b>子功能模块。</li>
            <li>负责处理<b>KeyBoard</b>与<b>Mouse</b>等外设的输入。</li>
            <li>负责通过<b>Config</b>配置文件进行程序配置</li>
        </ul>
    </p>
    <p>
        &emsp;<b>GUI</b>为用户界面。通常使用.NET框架，将一部分C++托管，使用WinForm，在CLR下运行。也可以使用Qt等其他框架。
    </p>
    <p> 
        &emsp;<b>Overlay</b>为悬浮界面，覆盖于目标程序窗口之上，背景透明且不响应任何消息，窗口消息透传至下一层即目标程序。主要负责透视框，外挂信
        息，游戏信息等的显示。使用DirectX11，基于微软的DirectXTK库，见<a href="https://github.com/microsoft/DirectXTK">Microsoft DirectXTK</a>。
    </p>
<h2>特性</h2>
    <ul>
        <li>支持x86/x64。</li>
        <li>包含AoB扫描，支持使用通配符进行模糊扫描。</li>
        <li>包含DLL注入器，支持DLL注入。</li>
        <li>内置DirectXTK，并进行了封装，便于Overlay设计。</li>
        <li>内置一个通用WinForm GUI，基于C++/CLR。</li>
    </ul>
<h2>使用方法</h2>
    <p>&emsp;1. 基于目标程序，向GameStruct添加外挂所需要的结构定义。</p>
    <p>&emsp;2. 根据各个功能子模块需求，编写逻辑并将需要的数据写入GameData，向相关配置参数写入Config。</p>
    <p>&emsp;3. 根据功能编写Manager，进行进程的信息获取，内存数据读取，处理外设输入，管理功能子模块等。</p>
    <p>&emsp;4. 编写用户UI，Overlay等。</p>
<h2>示例工程</h2>
    <p>&emsp;Counter-Strike: Global Offensive 外挂: <a href="https://github.com/CelestialTS/CSGO">CSGO External Hack</a></p>
    <p>&emsp;主要功能：ESP透视（DirectX Overlay），Glow透视，雷达显人，bHop，自动扳机，自动压枪，自瞄等</p>
<h2>声明</h2>
    <p>&emsp;
        <ul>
            <li>本程序仅为通用框架并非完整的具有功能的外挂，需要在该框架的基础上进行二次开发从而实现特定的功能。</li>
            <li>本程序作为外挂框架，并未将防检测纳入考量，请勿在受反外挂程序保护的服务器上使用。</li>
            <li>本程序作为游戏外挂使用时，仅限单机游戏使用，请勿在多人游戏中作弊。</li>
            <li>本程序作为其他用途时，请勿用于破坏网络安全，盗取个人信息，危害社会治安等用途，产生的一切后果与本程序无关。</li>
            <li>本程序仅限交流学习使用，请勿用于商业活动，转载或使用代码，请注明出处。</li>
        </ul>
    </p>
	<p>&emsp;欢迎对本程序进行完善修改，增加或删除内容，创建新分支等。</p>
    <p>&emsp;本程序基于MIT许可证发行，关于许可证请参阅LICENSE文件。</p>
<h2>关于</h2>
    <img src="https://github.com/CelestialTS/CTHackFramework/blob/master/res/logo.png" alt="Celestial Tech" width=400 height="=100">
    <p>更多详情请见天师苍邪科技官方网站: <a href="http://www.tianshicangxie.com">Celestial Tech</a></p>
    <p>Copyright © 2019 Celestial Tech</p>
       
