<?php

require __DIR__ . '/vendor/autoload.php';

use App\Command\ConvertBaseCommand;
use App\Command\ConvertToRomanCommand;
use Symfony\Component\Console\Application;

$app = new Application();

$app->add(new ConvertToRomanCommand());
$app->add(new ConvertBaseCommand());


$app->run();