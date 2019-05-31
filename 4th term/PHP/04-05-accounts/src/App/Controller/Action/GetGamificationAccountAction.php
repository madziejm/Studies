<?php


namespace App\Controller\Action;


use App\Exception\InvalidDescriptionException;
use App\Exception\InvalidEmailException;
use App\ValueObject\Uuid\GamificationAccountUuid;
use Exception;
use Slim\Http\Request;
use Slim\Http\Response;

final class GetGamificationAccountAction extends AbstractGamificationAccountAction
{

    public function __invoke(Request $request, Response $response, array $args)
    {
        try {
            $account = $this->gamificationAccountQueryService->findAccountByUuid(new GamificationAccountUuid($args['uuid']));
        } catch (\Exception $exception) {
            return $response->withJson(['status' => false, 'errors' => $exception->getMessage()])->withStatus(500);
        }
        return $response->withJson($account->jsonSerialize())->withStatus(200);
    }
}