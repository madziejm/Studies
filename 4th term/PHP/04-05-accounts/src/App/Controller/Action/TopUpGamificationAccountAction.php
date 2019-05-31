<?php


namespace App\Controller\Action;

use Exception;
use Slim\Http\Request;
use Slim\Http\Response;

final class TopUpGamificationAccountAction extends AbstractGamificationAccountAction
{

    public function __invoke(Request $request, Response $response, array $args)
    {
        $email = $request->getParam('email');
        $description = $request->getParam('description');

        try {
            $this->gamificationAccountService->createAccount($email, $description);
        } catch (\Exception $e) {
            return $response->withJson(['status' => false, 'errors' => 'Internal error occurred.'])->withStatus(500);
        }

        return $response->withJson(['status' => true])->withStatus(201);
    }
}