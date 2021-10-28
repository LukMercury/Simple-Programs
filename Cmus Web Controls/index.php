<!-- <!DOCTYPE html> -->
<html>
    <head>
        <style type="text/css">
            body {
                background-image: url('background.jpg');
                text-align: center;
                margin-top: 50px;
            }
            marquee{
                margin-bottom: 50px;
                font-size: 64px;
                font-weight: 800;
                color: #8ebf42;
                font-family: sans-serif;
            }
        </style>
    </head>

<body>

<marquee>
    <?php echo file_get_contents('song.txt'); ?>
</marquee>


<a class='prev' href="prev.php">
    <img src="prev-icon.png" width="128" height="128">
</a>

<a class='play-pause' href="play-pause.php">
    <img src="play-icon.png" width="128" height="128"> 
</a>

<a class='next' href="next.php">
    <img src="next-icon.png" width="128" height="128">
</a>


</body>

</html>