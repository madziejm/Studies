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
            ->setHelp('This command converts numbers provided by user to roman numerals.')
            ->addArgument('number',
                InputArgument::REQUIRED | InputArgument::IS_ARRAY,
                'Pass a number to convert');
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $numbers = $input->getArgument('number');
        $romanFormattedNumbers = "";

        foreach($numbers as $number)
        {
            $romanFormattedNumbers = $romanFormattedNumbers . " " . RomanNumberFormatter::format($number);
        }

        if(count($numbers) == 1)
            $message = sprintf('The number is: %s', $romanFormattedNumbers);
        else
            $message = sprintf('The numbers are: %s', $romanFormattedNumbers);

        $output->writeln($message);
    }
}