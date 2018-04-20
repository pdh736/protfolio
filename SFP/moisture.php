<?php
	$conn=mysqli_connect("localhost","root","kcci");
	mysqli_select_db($conn,"arduino");
	$result=mysqli_query($conn,"SELECT *FROM moisture");
?>

<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<meta http-equiv="refresh" content="30">
	<style type="text/css">
		.spec{
			text-align:center;
		}
		.con{
			text-align:left;
		}

	</style>
</head>
<body>
	<h1 align="center">Moisture</h1>
	  <div class="spec">
			# the sensor value description<br>
			# 0  ~300     dry soil<br>
			# 300~700     humid soil<br>
			# 700~950     in water<br>
		
	  </div>
	<table border='1' style="width=20%" align="center">
	<tr align="center">
		<th>ID</th>
		<th>DATE</th>
		<th>TIME</th>
		<th>MOISTURE</th>
	</tr>
	
	<?php
		while($row=mysqli_fetch_assoc($result) ){
		echo"<tr>";
		echo'<td>'.$row['id'].'</td>';
		echo'<td>'.$row['date'].'</td>';
		echo'<td>'.$row['time'].'</td>';
		echo'<td>'.$row['moisture'].'</td>';
		echo"</tr>";
		}
			
	?>
	</table>
		
</body>
</html>
