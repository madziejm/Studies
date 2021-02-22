var lastHoveredSquare='' //inner or outer
var countdownDate
var interval
var innerSquare
var squares
var remainingSquares
var allowedTime=100000 //ms

initialize();


function initialize()
{
    document.getElementById('new game').onclick=function(){initialize()};
    setSquares();
    setCountDown();
}

function setCountDownInterval(msRemaining)
{
    countdownDate = new Date(new Date().getTime()+msRemaining).getTime();
    interval = setInterval(function() {
        var now = new Date().getTime();
        var distance = countdownDate - now;
        var seconds = Math.round(distance/1000);
        
        document.getElementById("time").innerHTML = 'Jeszcze ' + seconds + 's do końca.';

        if (distance < 0 || remainingSquares==1) {
            clearInterval(interval);
            document.getElementById("time").innerHTML = "KONIEC";
            endGame()
        }
        }, 1000);
}

function endGame()
{
    for(var i=0; i<squares.length; i++)
    {
        squares[i].onmouseover = null;
        squares[i].onmouseout = null;
    }
    timeListItem=document.createElement('li')
    if(remainingSquares==1)
    {    
        timeListItem.textContent=((allowedTime-(countdownDate-new Date()))/1000)+'s'   
    }
    else
    timeListItem.textContent='Porażka'
    document.getElementById('timeList').appendChild(timeListItem);
    innerSquare.textContent='finito'
}

function setCountDown()
{
    setCountDownInterval(allowedTime)
}

function setSquares()
{
    squares=document.getElementsByClassName('square');
    remainingSquares=squares.length
    for(var i=0; i<squares.length; i++)
    {
        if(squares[i].id=='innerSquare')
        {
            innerSquare=squares[i]
            setSquareRandomPosition(innerSquare); //innerSquare size ought to be fixed
            innerSquare.textContent='Najpierw tutaj!';
            innerSquare.onmouseover = function(){innerOver()};
            innerSquare.onmouseout = function(){innerOut()};
            break;
        }
    }

    for(var i=0; i<squares.length; i++)
    {
        if(squares[i].id=='innerSquare')
            continue;
        let square=squares[i]
        square.setAttribute('data-hovered', 'false')
        squares[i].classList.remove('hovered')
        square.onmouseover = function(){outerOver(square)};
        square.onmouseout = function(){outerOut(square)};

        setSquareRandomSizeAndPosition(squares[i]);
        overlapswithprevious=false;
        do
        {
            overlapswithprevious=false;
            for(var j=0; j<i; j++)
            {
                if(overlap(squares[i], squares[j]))
                {
                    overlapswithprevious=true;
                    break;
                }
            }
            if(overlapswithprevious)
            {
                setSquareRandomSizeAndPosition(squares[i]);
            }

        }while(overlapswithprevious)
    }
}

function overlap(object1, object2)
{
    var rect1=object1.getBoundingClientRect();
    var rect2=object2.getBoundingClientRect();
    return !(rect1.right < rect2.left || 
             rect1.left > rect2.right || 
             rect1.bottom < rect2.top || 
             rect1.top > rect2.bottom);
}

function setSquareRandomPosition(square)
{
    square.style.left=(Math.random()*500).toFixed(0)+'px';
    square.style.top=(Math.random()*500).toFixed(0)+'px';
}

function setSquareRandomSizeAndPosition(square)
{
    setSquareRandomPosition(square)
    var size=Math.random()*75;
    square.style.height=(size+75).toFixed(0)+'px';
    square.style.width=square.style.height;
    square.style.lineHeight=square.style.height;
    if(35<size)
    {
        square.style.fontSize='20px';
    }
    else
    {
        square.style.fontSize='16px';
    }
}


function innerOver()
{
    msRemaining=countdownDate-new Date();
    clearInterval(interval)
    innerSquare.textContent=''
    for(var i=0; i<squares.length; i++)
    {
        if(squares[i]!=innerSquare)
        {
            squares[i].textContent='Teraz tu'
        }
    }
}

function innerOut(square)
{
    setCountDownInterval(msRemaining);
    lastHoveredSquare='inner';
}

function outerOver(square)
{
    innerSquare.textContent='Wróć tu!';
    for(var i=0; i<squares.length; i++)
    {
        if(squares[i]!=innerSquare)
        {
            squares[i].textContent=''
        }
    }
    // square.style.background='gray';
    if(lastHoveredSquare=='outer')
    {
        square.textContent='źle'; //najpierw czarny
    }
    if(square.getAttribute('data-hovered')=='true') //kara
    {
        countdownDate-=1000; //1 sekunda
    }
}

function outerOut(square)
{
    square.setAttribute('data-hovered', 'true')
    if(lastHoveredSquare=='inner'){
        square.classList.add('hovered');
        lastHoveredSquare='outer';
        // square
        remainingSquares--;
    }
}