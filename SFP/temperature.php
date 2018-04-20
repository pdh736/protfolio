<?php
   $conn=mysqli_connect("localhost","root","kcci");
	mysqli_select_db($conn,"arduino");
	$result=mysqli_query($conn,"SELECT *FROM temperature ORDER BY id DESC limit 20");
	
	$hour_ary=range(1,20);
	$min_ary=range(1,20);
	$sec_ary=range(1,20);
	$tem_ary=range(1,20);
	$date_ary=range(1,20);

	$i=1;
	while($row=mysqli_fetch_assoc($result)){
	      $seperator="-:";
		   $strTime=$row['time'];
		   $hour=intval(strtok($strTime,$seperator));
		   $min=intval(strtok($seperator));
		   $sec=intval(strtok($seperator));

			$date_ary[$i]=$row['date'];

			$hour_ary[$i]=$hour;
			$min_ary[$i]=$min;
			$sec_ary[$i]=$sec;
			$tem_ary[$i]=$row['temperature'];

			$i++;
	}
?>

<!DOCTYPE html>
<head>
	<meta charset="UTF-8">
	<meta http-equiv="refresh" content="10">
	<title>test highcharts</title>
</head>
<body>

<div id="container" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
<script src="https://code.highcharts.com/highcharts.js"></script>
<script src="https://code.highcharts.com/modules/exporting.js"></script>
    <script>
    Highcharts.chart('container', {
        chart: {
            type: 'line'
        },
        title: {
            text: 'TEMPERATURE'
        },
        subtitle: {
            text: 'Source: WorldClimate.com'
        },
        xAxis: {
            categories: [
				<?php 
				$j=20;
				while($j>0){
				echo '\''.$hour_ary[$j].'시'.$min_ary[$j].'분'.$sec_ary[$j].'초\','; 
				$j--;
				}
				?>
									
			],
            crosshair: true
        },
        yAxis: {
            min: 10,
            title: {
                text: 'Temperature(℃)'
            }
        },
        tooltip: {
				<?php
				$j=20;
            echo 'headerFormat: \'<span style="font-size:10px">'.$date_ary[$j].'<br>{point.key}</span><table>\',' ;
				$j--;
				?>
           // headerFormat: '<span style="font-size:10px">{point.key}</span><table>',
            pointFormat: '<tr><td style="color:{series.color};padding:0">{series.name}: </td>' +
                '<td style="padding:0"><b>{point.y:.1f} ℃</b></td></tr>',
            footerFormat: '</table>',
            shared: true,
            useHTML: true
        },
        plotOptions: {
            column: {
                pointPadding: 0.2,
                borderWidth: 0
            }
        },
		
        series: [{
            name: 'temperature',
            data: [
					<?php
						$j=20;
						while($j>0){
							
					echo ''.$tem_ary[$j].',';

							$j--;
						}
					?>
						
					]

        }]
    });        
    </script>
</body>
</html>
