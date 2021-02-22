<?php
/**
 * Created by PhpStorm.
 * User: maciej
 * Date: 2019-06-06
 * Time: 08:34
 */

namespace Router;

require __DIR__ . '/../../vendor/autoload.php';

use Command\Command;
use Exception\NoRouteFoundException;


class DirectRouter implements RouterInterface
{
    private $command2handler;

    /**
     * DirectRouter constructor.
     * @param $command2handler
     */
    public function __construct(Array $command2handler)
    {
        $this->command2handler = $command2handler;
//        print_r( $this->command2handler);
    }

    public function commandToHandler(Command $command)
    {
        $commandClass = get_class($command);

//        print_r($this->command2handler);
//
        if(array_key_exists($commandClass, $this->command2handler))
        {
            return $this->command2handler[$commandClass];
        }
        else
        {
            throw new NoRouteFoundException();
        }
    }
}