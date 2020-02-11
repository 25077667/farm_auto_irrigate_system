<!DOCTYPE HTML>
<!--
	Phantom by HTML5 UP
	html5up.net | @ajlkn
	Free for personal and commercial use under the CCA 3.0 license (html5up.net/license)
-->
<html>

<head>
    <title>Operate</title>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
    <link rel="stylesheet" href="assets/css/main.css" />
    <noscript>
        <link rel="stylesheet" href="assets/css/noscript.css" /></noscript>
</head>

<?php
$goals = readGoals();
function readGoals()
{
    if (($handle = fopen("Data/goal.csv", "r")) !== FALSE) {
        $goals = fgetcsv($handle);
    } else
        echo "read goal error";
    fclose($handle);
    return $goals;
}

function writeGaol($_value, $index)
{
    if ($_value == NULL || $index == NULL)
        return;
    $goals = readGoals();

    foreach($goals as $i)
        echo $i, " ";

    $goals[$index] = $_value;
    if (($handle1 = fopen("Data/goal.csv", "w+")) !== FALSE) {
        fputcsv($handle1, $goals);
    } else
        echo "write goal error";
    fclose($handle1);
    //header("Refresh:0");    //refresh page
}
// Be careful https://2www.w3school.com.cn/php/func_filesystem_fgetcsv.asp
?>

<body class="is-preload">
    <!-- Wrapper -->
    <div id="wrapper">

        <!-- Header -->
        <header id="header">
            <div class="inner">

                <!-- Logo -->
                <a href="index.html" class="logo">
                    <span class="symbol"><img src="images/logo.svg" alt="" /></span><span class="title">FARM HMI</span>
                </a>

                <!-- Nav -->
                <nav>
                    <ul>
                        <li><a href="#menu">Menu</a></li>
                    </ul>
                </nav>

            </div>
        </header>

        <!-- Menu -->
        <nav id="menu">
            <h2>Menu</h2>
            <ul>
                <li><a href="index.html">首頁</a></li>
                <li><a href="generic.php">資訊總表</a></li>
                <li><a href="operate.php">操作界面</a></li>
                <li><a href="smartHome.php">智慧家庭</a></li>
                <li><a href="history.php">過去紀錄</a></li>
                <li><a href="manual.html">使用手冊</a></li>
                <!--manual-->
            </ul>
        </nav>

        <!-- Main -->
        <div id="main">
            <div class="inner">
                <h1>操作界面</h1>
                <hr>
                <form action="" method="POST">
                    <div class='pigHouse'>
                        <!--https://www.w3schools.com/howto/howto_js_rangeslider.asp
					    https://blog.csdn.net/u013347241/article/details/51560290-->

                        <h2 style="text-align: center;">機器 0</h2>
                        <p>土壤濕度設定</p>
                        <?php
                        echo '<input type="range" min="1" max="100" value="', $goals[1], '" class="slider" name="pigMudWet">';
                        writeGaol($_POST["pigMudWet"], 1);
                        echo '<p>空氣溫度設定 </p>';
                        echo '<input type="range" min="20" max="36" value="', $goals[0], '" class="slider" name="pigAirWet">';
                        writeGaol($_POST["pigAirWet"], 0);
                        ?>
                        自動蓋網
                        <label class="switch">
                            <br>
                            <?php
                            echo '<input type="checkbox" value="', $goals[4], '" name="pigNet">';
                            writeGaol($_POST["pigNet"], 4);
                            ?>
                            <span class="sliderNet round"></span>
                        </label> 手動蓋網
                    </div>
                    <!--<div class='sugarCane'>
                        <h2 style="text-align: center;">甘蔗</h2>
                        <p>土壤濕度設定</p>
                        <input type="range" min="1" max="100" value="50" class="slider" id='sugarMoodWet'>
                        <p>空氣溫度設定 </p>
                        <input type="range" min="1" max="100" value="50" class="slider" id='sugarAirWet'> 自動蓋網
                        <label class="switch">
                            <br>
                            <input type="checkbox" value="0" id="sugarNet">
                            <span class="sliderNet round"></span>
                        </label> 手動蓋網
                    </div>
                    <div class='backyard'>
                        <h2 style="text-align: center;">後院</h2>
                        <p>土壤濕度設定</p>
                        <input type="range" min="1" max="100" value="50" class="slider" id='backyardMoodWet'>
                        <p>空氣溫度設定 </p>
                        <input type="range" min="1" max="100" value="50" class="slider" id='backyardAirWet'> 自動蓋網
                        <label class="switch">
                            <br>
                            <input type="checkbox" value="0" id="backyardNet">
                            <span class="sliderNet round"></span>
                        </label> 手動蓋網
                    </div>-->
                    <input type=submit value="送出">
                </form>>
                <div id="time"> </div>
                <br>
                <button onclick='javascript:history.go(-1)'>回上頁</button>
            </div>
        </div>

        <!-- Footer -->
        <footer id="footer">
            <div class="inner">
                <section>
                    <h2>Follow</h2>
                    <ul class="icons">
                        <li>
                            <a href="https://fb.com/momom0m0" class="icon brands style2 fa-facebook-f" target="_blank" rel="noopener noreferrer">
                                <span class="label">Facebook</span>
                            </a>
                        </li>
                        <li>
                            <a href="https://github.com/25077667" class="icon brands style2 fa-github" target="_blank" rel="noopener noreferrer">
                                <span class="label">GitHub</span>
                            </a>
                        </li>
                        <li>
                            <a href="0905273401" class="icon solid style2 fa-phone" target="_blank" rel="noopener noreferrer">
                                <span class="label">Phone</span>
                            </a>
                        </li>
                        <li>
                            <a href="zxc25077667@gmail.com" class="icon solid style2 fa-envelope" target="_blank" rel="noopener noreferrer">
                                <span class="label">Email</span>
                            </a>
                        </li>
                    </ul>
                </section>
                <ul class="copyright">
                    <li>&copy; Untitled. All rights reserved</li>
                    <li>Design: <a href="http://html5up.net" target="_blank" rel="noopener noreferrer">HTML5 UP</a></li>
                    <li>Modify: <a href="https://fb.com/momom0m0" target="_blank" rel="noopener noreferrer">SCC</a></li>
                </ul>
            </div>
        </footer>

    </div>

    <!-- Scripts -->
    <script src="assets/js/jquery.min.js"></script>
    <script src="assets/js/browser.min.js"></script>
    <script src="assets/js/breakpoints.min.js"></script>
    <script src="assets/js/util.js"></script>
    <script src="assets/js/main.js"></script>
    <script src="assets/js/moment.js"></script>
</body>

</html>