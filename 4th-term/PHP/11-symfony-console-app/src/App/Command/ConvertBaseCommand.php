<?php

namespace App\Command;

use App\Number\Number;
use App\Number\NumberFormatter\RomanNumberFormatter;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Input\InputArgument;

class ConvertBaseCommand extends Command
{
    protected function configure()
    {
        $this->setName('convert-base')
            ->setDescription('Prints provided numbers using roman numerals')
            ->setHelp('This command converts numbers provided by user to roman numerals.');

        $this->addArgument('input_base',
            InputArgument::REQUIRED,
            'Pass input numbers\' base');
        $this->addArgument('target_base',
                InputArgument::REQUIRED,
            'Pass base to convert to');
        $this->addArgument('numbers',
                InputArgument::REQUIRED | InputArgument::IS_ARRAY,
                'Pass a numbers to convert');
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        if($input->getArgument('target_base') != "10")
        {
            $output->writeln("Only target base of 10 is supported");
            return 1;
        }

        $numbers = $input->getArgument('numbers');
        $formattedNumbers = "";

        foreach($numbers as $number)
        {
            $numberDecimalValue = Number::createInstanceOfString($input->getArgument('input_base'), $number)->toInt();
            $formattedNumbers = $formattedNumbers . " " . $numberDecimalValue;
        }

        $message = "";
        if(!$output->isQuiet())
        {
            if(count($numbers) == 1)
                $message = 'The number is:';
            else
                $message = 'The numbers are:';
        }
        $message .= $formattedNumbers;

        $output->writeln($message);
    }
}