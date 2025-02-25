# Collang

A programming language.

## Syntax

### Grammar

```
StatementList->		Statement*
Conditional->		("if" | "while") Logical (Block | Statement)
Block->				"{" StatementList "}"
Statement-> 		(Assignment | Reassignment | Expression) ";"
Expression-> 		TernaryConditional
Assignment-> 		IDENTIFIER ":" IDENTIFIER "=" Expression
Reassignment-> 		IDENTIFIER ("=" | "+=" | "-=" | "*=" | "/=" | "<<=" | ">>=" | "|=" | "&=" | "~=" | "%=") Expression
TernaryCondition->	Logical "?" Expression "," Expression
Logical->			Relational (("and" | "or") Relational)*
Relational->		BitwiseOr (("<" | ">" | "<=" | ">=" | "==" | "!=") BitwiseOr)*
BitwiseOr-> 		BitwiseXor ("|" BitwiseXor)*
BitwiseXor-> 		BitwiseAnd ("*|" BitwiseAnd)*
BitwiseAnd-> 		Shift ("&" Shift)*
Shift-> 			Additive (("<<" | ">>") Additive)*
Additive-> 			Multiplicative (("+" | "-") Multiplicative)*
Multiplicative-> 	Unary (("*" | "/" | "%") Unary)*
Unary-> 			("+" | "-" | "!" | "~")* Primary
Primary-> 			IDENTIFER | CONSTANT | "(" Expression ")"
```

Simple return 0 program

```css
fnc Entry(): tWrd
{
	ret 0;
}
```
