<?php
/**
 * Created by PhpStorm.
 * User: maciej
 * Date: 2019-06-06
 * Time: 08:12
 */

namespace Command;


class PingCommand extends DuckCommand
{

    public function quack()
    {
        echo "Ping!" . PHP_EOL;
    }
}