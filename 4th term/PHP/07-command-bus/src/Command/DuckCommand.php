<?php
/**
 * Created by PhpStorm.
 * User: maciej
 * Date: 2019-06-06
 * Time: 08:02
 */

namespace Command;


abstract class DuckCommand extends Command
{
    abstract public function quack();
}