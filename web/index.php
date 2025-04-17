<?php
    ini_set('display_errors', 1);
    ini_set('display_startup_errors', 1);
    error_reporting(E_ALL);

    require_once 'database.php';
    require_once 'env.php';

    $database = new Database($ENV_HOST, $ENV_DB_NAME, $ENV_USERNAME, $ENV_PASSWORD);

    if(!empty($_GET['device'])) {
        $device = $_GET['device'];
    } else {
        $device = 'raspberry_device';
    }

    $req = $database->db->prepare('SELECT * FROM values_devices WHERE device = ? LIMIT 2500');
    $req->execute(array($device));
    $values = $req->fetchAll()
?>
<html>
    <head>
        <meta charset="utf-8">
        <title>IOT Panel</title>
        <style type="text/css">
            body {
                background: #F2F2F7;
                font-family: Arial;
                padding: 20px;
            }
        </style>
    </head>
    <body>
        <h1 style="display: flex; align-items: center"><?php echo $device; ?> <span style="width: 10px; height: 10px; background: green; display: block; margin-left: 15px; border-radius: 15pxsssssssssssssssss;"></span></h1>
        <div style="display: flex; flex-wrap: wrap; justify-content: space-around;">
            <?php
                $firstValue = "";

                //var_dump($req->fetchAll());

                foreach($values as $key => $row) {
                    if($key == 0) {
                        $firstValue = $row['value_name'];
                    } elseif($row['value_name'] == $firstValue) {
                        break;
                    }
                    ?>
                        <div id="my<?php echo $row['value_name']; ?>" style="width:700px; height:400px"></div>
                    <?php
                }
            ?>
        </div>
    </body>
    <script src="https://www.gstatic.com/charts/loader.js"></script>
    <script>
        google.charts.load('current',{packages:['corechart']});
        google.charts.setOnLoadCallback(drawChart);

        function drawChart() {
            <?php
                //var_dump($values);
                $values_array = array();

                foreach($values as $key => $row) {
                    if(empty($values_array[$row['value_name']])) {
                        $values_array[$row['value_name']] = "";
                    }
                    $req_date = new DateTime($row['date_req']);

                    $date = $req_date->getTimestamp() * 1000;

                    $values_array[$row['value_name']] .= "[new Date(".$date."),".$row['value_data']."],";
                }

                $firstValue = "";

                foreach($values as $key => $row) {
                    if($key == 0) {
                        $firstValue = $row['value_name'];
                    } elseif($row['value_name'] == $firstValue) {
                        break;
                    }


                    ?>
                        const data<?php echo $row['value_name']; ?> = new google.visualization.DataTable();
                        data<?php echo $row['value_name']; ?>.addColumn('date', 'Date');
                        data<?php echo $row['value_name']; ?>.addColumn('number', 'Valeur');
                        
                        data<?php echo $row['value_name']; ?>.addRows([<?php echo $values_array[$row['value_name']]; ?>]);

                        const options<?php echo $row['value_name']; ?> = {
                            title: '<?php echo $row['value_name']; ?>',
                            hAxis: {title: 'Date'},
                            vAxis: {title: 'Value'},
                            legend: 'none'
                        };

                        const chart<?php echo $key; ?> = new google.visualization.LineChart(document.getElementById('my<?php echo $row['value_name']; ?>'));
                        chart<?php echo $key; ?>.draw(data<?php echo $row['value_name']; ?>, options<?php echo $row['value_name']; ?>);
                    <?php
                }
            ?>
        }
    </script>
</html>