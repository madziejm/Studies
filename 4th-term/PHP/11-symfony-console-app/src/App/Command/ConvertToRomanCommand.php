<?php

namespace App\Command;

use App\Number\NumberFormatter\RomanNumberFormatter;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Input\InputArgument;

class ConvertToRomanCommand extends Command
{
    protected function configure()
    {
        $this->setName('convert-to-roman')
            ->setDescription('Prints provided numbers using roman numerals')
            ->setHelp('This command converts numbers provided by user to roman numerals.');

        $this->addArgument('numbers',
            InputArgument::REQUIRED | InputArgument::IS_ARRAY,
            'Pass a numbers to convert');
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $numbers = $input->getArgument('numbers');
        $romanFormattedNumbers = "";

        foreach($numbers as $number)
        {
            $romanFormattedNumbers = $romanFormattedNumbers . " " . RomanNumberFormatter::format($number);
        }

        $message = "";
        if(!$output->isQuiet())
        {
            if(count($numbers) == 1)
                $message .= 'The number is:';
            else
                $message .= 'The numbers are:';
        }
        $message = $message . $romanFormattedNumbers;

        $output->writeln($message);
    }
}