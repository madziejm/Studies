<?php
/**
 * Created by PhpStorm.
 * User: maciej
 * Date: 2019-06-06
 * Time: 08:15
 */

namespace Router;


use Command\Command;

interface RouterInterface
{
    public function commandToHandler(Command $command);
}