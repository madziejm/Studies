<?php


namespace App\Controller\Action;


use App\QueryService\GamificationAccountQueryService;
use App\QueryService\GamificationAccountQueryServiceInterface;
use App\Service\GamificationAccountServiceInterface;
//use Pimple\Container;
use Slim\Container;
use Slim\Http\Request;
use Slim\Http\Response;

/**
 * Class AbstractGamificationAccountAction
 * @package App\Controller\Action
 */
abstract class AbstractGamificationAccountAction
{
    /**
     * @var Container
     */
    protected $container;

    /**
     * @var GamificationAccountQueryServiceInterface
     */
    protected $gamificationAccountQueryService;

    /**
     * @var GamificationAccountServiceInterface
     */
    protected $gamificationAccountService;

    /**
     * AbstractGamificationAccountAction constructor.
     * @param Container $container
     */
    public function __construct(Container $container)
    {
//        $this->$container = $container;
        $this->gamificationAccountQueryService = $container['gamificationAccountQueryService'];
        $this->gamificationAccountService = $container['gamificationAccountService'];
    }

    public abstract function __invoke(Request $request, Response $response, array $args);

}