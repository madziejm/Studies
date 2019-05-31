<?php


namespace App\Controller\Action;

use App\ValueObject\Uuid\GamificationAccountUuid;
use Exception;
use Slim\Http\Request;
use Slim\Http\Response;

final class ActivateGamificationAccountAction extends AbstractGamificationAccountAction
{

    public function __invoke(Request $request, Response $response, array $args)
    {
        try {
            $this->gamificationAccountService->activateAccount(new GamificationAccountUuid(new GamificationAccountUuid($request->getParam('uuid'))));
        } catch (\Exception $exception) {
            return $response->withJson(['status' => false, 'errors' => $exception->getMessage()])->withStatus(500);
        }
        return $response->withJson(['status' => true])->withStatus(200);

//        return $response->withJson(['status' => true, 'balance' => $this->gamificationAccountQueryService->findAccountByUuid(new GamificationAccountUuid($request->getParam('uuid')))->getBalance()])->withStatus(200);
    }
}