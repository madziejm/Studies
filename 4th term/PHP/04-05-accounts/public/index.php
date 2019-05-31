<?php

use App\Controller\Action\ActivateGamificationAccountAction;
use App\Controller\Action\CreateGamificationAccountAction;
use App\Controller\Action\DeactivateGamificationAccountAction;
use App\Controller\Action\GetAllGamificationAccountsAction;
use App\Controller\Action\GetGamificationAccountAction;
use App\Controller\Action\TopUpGamificationAccountAction;
use App\Policy\GamificationAccountPolicy;
use App\QueryService\GamificationAccountQueryService;
use App\Repository\FileGamificationAccountRepository;
use App\Service\GamificationAccountService;
use Monolog\Handler\StreamHandler;
use Monolog\Logger;
use \Psr\Http\Message\ServerRequestInterface as Request;
use \Psr\Http\Message\ResponseInterface as Response;
use Slim\App;

require '../vendor/autoload.php';

$config['displayErrorDetails'] = true;
$config['addContentLengthHeader'] = false;

//$config['db']['host']   = 'localhost';
//$config['db']['user']   = 'user';
//$config['db']['pass']   = 'password';
//$config['db']['dbname'] = 'exampleapp';

$app = new App(['settings' => $config]);

$container = $app->getContainer();

$container['logger'] = function($c) {
    $logger = new Logger('my_logger');
    $file_handler = new StreamHandler('../logs/app.log');
    $logger->pushHandler($file_handler);
    return $logger;
};

$container['gamificationAccountRepository'] = function ($c){
    return new FileGamificationAccountRepository();
};

$container['gamificationAccountService'] = function ($c){
    return new GamificationAccountService(
        $c['gamificationAccountRepository'],
        new GamificationAccountPolicy($c['gamificationAccountRepository'])
    );
};

$container['gamificationAccountQueryService'] = function ($c){
    return new GamificationAccountQueryService($c['gamificationAccountRepository']);
};


//$container['db'] = function ($c) {
//    $db = $c['settings']['db'];
//    $pdo = new PDO('mysql:host=' . $db['host'] . ';dbname=' . $db['dbname'],
//        $db['user'], $db['pass']);
//    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
//    $pdo->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);
//    return $pdo;
//};

$app->post('/api/account/create',
    CreateGamificationAccountAction::class);

$app->get('/api/account/all',
    GetAllGamificationAccountsAction::class);

$app->post('/api/account/topup',
    TopUpGamificationAccountAction::class);

$app->post('/api/account/activate',
    ActivateGamificationAccountAction::class);

$app->post('/api/account/deactivate',
    DeactivateGamificationAccountAction::class);

$app->get('/api/account/{uuid}',
    GetGamificationAccountAction::class);


$app->get('/api/hello/{name}', function (Request $request, Response $response, array $args) {
    $name = $args['name'];
    $response->getBody()->write("Hello, $name");
    $this->logger->addInfo('Hello happened');

    return $response;
});
//
//$app->post('/api/ticket/new', function (Request $request, Response $response) {
//    $data = $request->getParsedBody();
//    $ticket_data = [];
//    $ticket_data['title'] = filter_var($data['title'], FILTER_SANITIZE_STRING);
//    $ticket_data['description'] = filter_var($data['description'], FILTER_SANITIZE_STRING);
//    $response->getBody()->write("Hello!");
//    $this->logger->addInfo('Ticket happened');
//
//    return $response;
//});


// quite important to do so:
$app->run();
