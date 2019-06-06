<?php
/**
 * Created by PhpStorm.
 * User: maciej
 * Date: 2019-06-06
 * Time: 10:30
 */

namespace Handler;

use Command\PingCommand;

class PingHandler extends Handler
{

    public function __invoke(PingCommand $c)
    {
        $c->quack();
    }
}