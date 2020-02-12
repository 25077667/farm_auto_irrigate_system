<!DOCTYPE HTML>
<!--
	Phantom by HTML5 UP
	html5up.net | @ajlkn
	Free for personal and commercial use under the CCA 3.0 license (html5up.net/license)
-->
<html>

<head>
    <title>Generic</title>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
    <link rel="stylesheet" href="assets/css/main.css" />
    <noscript>
        <link rel="stylesheet" href="assets/css/noscript.css" /></noscript>
</head>

<?php
require("tailFileSample.php");
class BundleData
{
    public $ID;
    public $temperature;
    public $hermidity;
    public $uvIndex;

    function getID()
    {
        return $this->ID;
    }
    function getTemperature()
    {
        return $this->temperature;
    }
    function getHermidity()
    {
        return $this->hermidity;
    }
    function getUV()
    {
        return $this->uvIndex;
    }

    function __construct($i)
    {
        $lines = 5;
        $tail = explode("\n", tailCustom("Data/history", $lines, true));
        $tailCSV = array();
        foreach ($tail as $j)
            array_push($tailCSV, str_getcsv($j));

        foreach ($tailCSV as $j) {
            /*$counter = 0;
            $doAssign = false;
            foreach ($j as $k) {
                if ($counter == 1 && $k == $i)
                    $doAssign = true;
                else if ($doAssign && $counter == 2)
                    $this->temperature = $k;
                else if ($doAssign && $counter == 3)
                    $this->hermidity = $k;
                else if ($doAssign && $counter == 4)
                    $this->uvIndex = $k;
                $counter++;
            }*/

            // Why here can't work?
            if ($j[1] == $i) {
                $this->temperature = $j[2];
                $this->hermidity = $j[3];
                $this->uvIndex = $j[4];
                break;
            }
        }
    }
}

$machine = array();
for ($i = 0; $i < 3; $i++) {
    $newMachine = new BundleData($i);
    array_push($machine, $newMachine);
}

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
                <li><a href="index.html">資訊總表</a></li>
                <li><a href="index.html">操作界面</a></li>
                <li><a href="index.html">智慧家庭</a></li>
                <li><a href="history.php">過去紀錄</a></li>
                <li><a href="elements.html">使用手冊</a></li>
                <!--manual-->
            </ul>
        </nav>

        <!-- Main -->
        <div id="main">
            <div class="inner">
                <h1>資訊總表</h1>
                <hr>
                <div class='pigHouse'>
                    <!--https://www.w3schools.com/howto/howto_js_rangeslider.asp
					https://blog.csdn.net/u013347241/article/details/51560290
				-->
                    <h2 style="text-align: center;">機器 0 </h2>
                    <div class="progress">
                        <div class="progress-bar progress-bar-info progress-bar-striped active" role="progressbar" aria-valuemin="0" aria-valuemax="99" style="width:<?php echo $machine[0]->getHermidity(), '%'; ?>">
                            濕度
                        </div>
                    </div>
                    <div class="progress">
                        <div class="progress-bar progress-bar-info progress-bar-striped active" role="progressbar" aria-valuemin="0" aria-valuemax="99" style="width:<?php echo $machine[0]->getTemperature() / 36 * 100, '%'; ?>">
                            溫度
                        </div>
                    </div>
                    <div class="progress">
                        <div class="progress-bar progress-bar-info progress-bar-striped active" role="progressbar" aria-valuemin="0" aria-valuemax="99" style="width:<?php echo $machine[0]->getUV() / 11 * 100, '%'; ?>">
                            紫外線
                        </div>
                    </div>

                </div>
                <!--
                <div class='sugarCane'>
                    <h2 style="text-align: center;">甘蔗</h2>
                    <div class="progress">
                        <div class="progress-bar progress-bar-info progress-bar-striped active" role="progressbar" aria-valuemin="0" aria-valuemax="99" style="width:50%">
                            濕度
                        </div>
                    </div>
                    <div class="progress">
                        <div class="progress-bar progress-bar-info progress-bar-striped active" role="progressbar" aria-valuemin="0" aria-valuemax="99" style="width:50%">
                            溫度
                        </div>
                    </div>
                    <div class="progress">
                        <div class="progress-bar progress-bar-info progress-bar-striped active" role="progressbar" aria-valuemin="0" aria-valuemax="99" style="width:50%">
                            紫外線
                        </div>
                    </div>
                </div>
                <div class='backyard'>
                    <h2 style="text-align: center;">後院</h2>
                    <div class="progress">
                        <div class="progress-bar progress-bar-info progress-bar-striped active" role="progressbar" aria-valuemin="0" aria-valuemax="99" style="width:50%">
                            濕度
                        </div>
                    </div>
                    <div class="progress">
                        <div class="progress-bar progress-bar-info progress-bar-striped active" role="progressbar" aria-valuemin="0" aria-valuemax="99" style="width:50%">
                            溫度
                        </div>
                    </div>
                    <div class="progress">
                        <div class="progress-bar progress-bar-info progress-bar-striped active" role="progressbar" aria-valuemin="0" aria-valuemax="99" style="width:50%">
                            紫外線
                        </div>
                    </div>
                </div>
                -->
                <?php echo '<div id="time">', date('d-m-Y h:i:s'), '</div>'; ?>
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