Expression:
    Term
    Term + Expression
    Term - Expression

Term:
    Primary
    Term * Primary
    Term / Primary

Primary:
    VariableX
    Number
    ( Expression )
    Function ( Expression )
