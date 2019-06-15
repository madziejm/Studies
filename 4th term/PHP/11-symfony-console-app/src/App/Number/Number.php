<?php

namespace Number;

class Number
{
    var $base;
    var $numericalValue;
    var $stringRepresentation;

    static function digitCharToValue($char)
    {
        $digits = array(
            "1" => 1,
            "2" => 2,
            "3" => 3,
            "4" => 4,
            "5" => 5,
            "6" => 6,
            "7" => 7,
            "8" => 8,
            "9" => 9,
            "a" => 10,
            "b" => 11,
            "c" => 12,
            "d" => 13,
            "e" => 14,
            "f" => 15,
            "A" => 10,
            "B" => 11,
            "C" => 12,
            "D" => 13,
            "E" => 14,
            "F" => 15,
        );
        return $digits[$char];
    }

    static function digitValueToChar($digitValue)
    {
        $digits = array(
            1 => '1',
            2 => '2',
            3 => '3',
            4 => '4',
            5 => '5',
            6 => '6',
            7 => '7',
            8 => '8',
            9 => '9',
            10 => '10',
            11 => '11',
            12 => '12',
            13 => '13',
            14 => '14',
            15 => '15',
        );
        return $digits[$digitValue];
    }

    /**
     * @return int
     */
    public function toInt()
    {
        if (!is_null($this->numericalValue))
//            return 0;
            return $this->numericalValue;
        $this->numericalValue = 0;
//        echo gettype($this->number);
//        echo gettype($this->base);
//        echo $this->number;
        $digits = str_split($this->stringRepresentation);
        foreach ($digits as $digit) {
            $this->numericalValue = $this->numericalValue * $this->base;
//            echo "base=";
//            echo $this->base;
//            echo "\n";
//            echo $this->number;
            $this->numericalValue = self::digitCharToValue($digit) + $this->numericalValue;
//            echo self::digitCharToValue($digit);
        }
        return $this->numericalValue;
    }

    public function printNumber()
    {
        echo $this->toInt();
        echo "\n";
    }

    /**
     * Number constructor.
     * @param base
     */
    private function __construct()
    {
//        if (false === $this->mutable) {
//            throw new \BadMethodCallException('Constructor called twice.');
    }

    public static function createInstanceOfString($base, $stringRepresentation)
    {
        $instance = new self();

        $instance->base = $base;
//        print "instance->base";
//        print $instance->base;
        $instance->stringRepresentation = $stringRepresentation;
//        $instance->numericalValue = null;
        $instance->toInt();
        return $instance;
    }

    private static function createInstanceOfNumericalValue($base, $numericalValue)
    {
        $instance = new self();

        $instance->base = $base;
//        $instance->stringRepresentation = $stringRepresentation;
        $instance->numericalValue = $numericalValue;
//        $instance->getNumber();
        while ($numericalValue > 0) {
            $instance->stringRepresentation = self::digitValueToChar($numericalValue % $base);
            $numericalValue /= $base;
        }
        return $instance;
    }

    /**
     * @return mixed
     */
    public function getBase()
    {
        return $this->base;
    }

    /**
     * @return mixed
     */
    public function getStringRepresentation()
    {
        return $this->stringRepresentation;
    }

    public function addNumber(Number $b)
    {
        return self::createInstanceOfNumericalValue($this->base, $this->toInt() + $b->toInt());
    }

}