<?php
/**
 * Created by PhpStorm.
 * User: maciej
 * Date: 2019-06-05
 * Time: 23:36
 */

namespace CommandBus;

require __DIR__ . '/../../vendor/autoload.php';

use Command\Command;
use Exception\NoRouteFoundException;
use Router\DirectRouter;

class CommandBus
{
    private $router;

    /**
     * CommandBus constructor.
     * @param $command2handler
     */
    public function __construct($command2handler)
    {
        $this->router = new DirectRouter($command2handler);
    }

    public function dispatch(Command $command)
    {
        try {
            $commandHandlerClass = $this->router->commandToHandler($command);
        } catch (NoRouteFoundException $e) {
            throw $e;
        }

        $commandHandler = new $commandHandlerClass();

        $commandHandler($command);
        return;
    }

}