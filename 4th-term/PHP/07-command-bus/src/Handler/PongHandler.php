<?php
/**
 * Created by PhpStorm.
 * User: maciej
 * Date: 2019-06-06
 * Time: 10:30
 */

namespace Handler;

use Command\PongCommand;

class PongHandler extends Handler
{

    public function __invoke(PongCommand $c)
    {
        $c->quack();
    }
}