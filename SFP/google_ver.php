<?php
   $conn=mysqli_connect("localhost","root","kcci");
   mysqli_select_db($conn,"arduino");
   $result=mysqli_query($conn,"SELECT *FROM temperature ORDER BY id DESC limit 20");
?>



<!DOCTYPE HTML>
<html>
	<head>
		<meta charset="UTF-8">
		<meta http-equiv="refresh" content="30">
	</head>
	<body>
	     <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
		      <script type="text/javascript">

				google.charts.load('current', {packages: ['corechart', 'line']});
				google.charts.setOnLoadCallback(drawBackgroundColor);

				var chartDateformat     = 'yy년MM월dd일';
				var chartTimeformat     = 'hh시mm분';
				var dataRow=[];
				function drawBackgroundColor() {
					var dataRow=[];
					var data = new google.visualization.DataTable();
					data.addColumn('datetime', 'X');
					data.addColumn('number', 'Temperature');


					  <?php
						while($row=mysqli_fetch_assoc($result)){
							$seperator="-:";
							$strTime=$row['time'];
							$hour=intval(strtok($strTime,$seperator));
							$min=intval(strtok($seperator));
							$sec=intval(strtok($seperator));
							$strDate=$row['date'];
							$year=intval(strtok($strDate,$seperator));
							$month=intval(strtok($seperator))-1;
							$day=intval(strtok($seperator));
							echo	'dataRow = [new Date('.$year.','.$month.' ,'.$day.','.$hour.','.$min.','.$sec.'),'.$row['temperature'].'];';
							echo	"data.addRow(dataRow);";
							//new Date(\''.$row['date'].'\')];';
						}
					  ?>
				
					var options = {
						hAxis: {
		//				format: chartDateformat,
	//					format: chartTimeformat,
						title: 'Time'
						},
						vAxis              : {minValue: 70,viewWindow:{min:0},gridlines:{count:-1},textStyle:{fontSize:12}},
						backgroundColor: '#f1f8e9'
					};

					var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
					chart.draw(data, options);
				}
	
		</script>
		<div id="chart_div"></div>
	</body>
</html>
