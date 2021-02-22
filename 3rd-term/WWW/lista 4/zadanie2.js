function indicateWrongField(field)
{
}

function validateInputField(field)
{
    //returns true when field is correct or error mesage otherwise
    console.log(field);
    console.log(field.value);
    var fieldType=field.getAttribute('data-form-field-type');
    if(fieldType===null)
        return true;
    switch(fieldType)
    {
        case 'name':
        re=(new RegExp(/^([A-Za-z]{2,40})$/));
            if(re.test(field.value)) {
                field.setCustomValidity("");
                return true
            }
            else {
                field.setCustomValidity("Invalid field.");
                return 'Invalid name field'
            }
            // break;

        case 'account number':
            if(field.value.match('[0-9]{26}')) {
                field.setCustomValidity("");
                return true
            }
            else {
                field.setCustomValidity("Invalid account number");
                return 'Invalid account number'
            }
            // break;

        case 'email':
            re = /\S+@\S+\.\S+/;
            if(re.test(field.value)) {
                field.setCustomValidity("");
                return true
            }
            else {
                field.setCustomValidity("Invalid email");
                return 'Invalid email field'
            }
            // break;

        case 'postcode':
            if(field.value.match('^[0-9]{2}[\-][0-9]{3}$')) {
                field.setCustomValidity("");
                return true
            }
            else {
                field.setCustomValidity("Invalid field.");
                return 'Invalid name field'
            }
            // break;

        case 'amount':
            if(field.value.match('^[0-9]+[,][0-9]{2}$')) {
                field.setCustomValidity("");
                return true
            }
            else {
                field.setCustomValidity("Invalid amount");
                return 'Invalid amount'
            }
            // break;

        case 'date from now':
        if((new Date())<=(new Date(field.value))) {
            field.setCustomValidity("");
            return true
        }
        else {
            field.setCustomValidity("Invalid date ");
            return 'Invalid date'
        }
        // break;

    }
    return '';
}

function validateInput(form)
{
    // console.log('teraz będzie formularz')
    // console.log(form);
    for(var i=0; i<form.elements.length; i++) //to jest window, a nie form (?)
    {
        // console.log(form[i]);
        // console.log('this');
        message=validateInputField(form.elements[i]);
        console.log(message);
        if(message!=true)
            alert(message)
    }
}

function test()
{
    alert("test");
}

function initializeForms()
{
    var forms=document.getElementsByTagName("form");
    // var forms=document.forms
    for(var i=0; i<forms.length; i++)
    {
        // console.log(i);
        // console.log(forms[i])
        let form=forms[i];
        forms[i].onsubmit = function() {validateInput(form)};
        for(var j=0; j<forms[i].elements.length; j++)
        {
            let input=form.elements[j]
            form.elements[j].onchange = function() {validateInputField(input)};//{validateInput()};
        }

        // console.log(forms[i])
    }
}
initializeForms();