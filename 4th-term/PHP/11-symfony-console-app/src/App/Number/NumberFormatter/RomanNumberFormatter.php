<?php
namespace App\Number\NumberFormatter;

class RomanNumberFormatter implements NumberFormatterInterface
{

    /**
     * @param $number
     * @return string
     */
    public static function format($number)
    {
        $stringRepresentation = '';
        $values = array(
            1000 => 'M',
            900 => 'CM',
            500 => 'D',
            400 => 'CD',
            100 => 'C',
            90 => 'XC',
            50 => 'L',
            40 => 'XL',
            10 => 'X',
            9 => 'IX',
            5 => 'V',
            4 => 'IV',
            1 => 'I',
        );

        foreach ($values as $value => $character) {
            while ($number >= $value) {
                $stringRepresentation .= $character;
                $number -= $value;
            }
        }
        return $stringRepresentation;
    }
}