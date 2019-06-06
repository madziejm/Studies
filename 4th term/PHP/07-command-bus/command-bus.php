<?php
/**
 * Created by PhpStorm.
 * User: maciej
 * Date: 2019-06-06
 * Time: 09:30
 */

require_once __DIR__ . "/vendor/autoload.php";

use Command\PingCommand;
use Command\PongCommand;
use CommandBus\CommandBus;
use Handler\PingHandler;
use Handler\PongHandler;

$array = [
    PingCommand::class => PingHandler::class,
    PongCommand::class => PongHandler::class];
//print_r($array);
$commandBus = new CommandBus($array);

$commandBus->dispatch(new PingCommand());
$commandBus->dispatch(new PongCommand());
$commandBus->dispatch(new PingCommand());
$commandBus->dispatch(new PingCommand());
$commandBus->dispatch(new PongCommand());
$commandBus->dispatch(new PongCommand());