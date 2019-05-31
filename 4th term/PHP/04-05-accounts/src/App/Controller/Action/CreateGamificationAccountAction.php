<?php


namespace App\Controller\Action;


use App\Exception\InvalidDescriptionException;
use App\Exception\InvalidEmailException;
use Exception;
use Slim\Http\Request;
use Slim\Http\Response;

final class CreateGamificationAccountAction extends AbstractGamificationAccountAction
{

    public function __invoke(Request $request, Response $response, array $args)
    {
        $email = $request->getParam('email');
        $description = $request->getParam('description');

        try {
            $this->gamificationAccountService->createAccount($email, $description);
        } catch (InvalidEmailException $e) {
            return $response->withJson(['status' => false, 'errors' => 'Invalid email.'])->withStatus(500);
        } catch (InvalidDescriptionException $e) {
            return $response->withJson(['status' => false, 'errors' => 'Invalid description.'])->withStatus(500);
        } catch (\Exception $e) {
            return $response->withJson(['status' => false, 'errors' => 'Internal error occurred.'])->withStatus(500);
        }
        //TODO return ID in the response
        return $response->withJson(['status' => true])->withStatus(201);
    }
}