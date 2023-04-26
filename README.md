# Ozymandias

Ozymandias is a Domain-Specific Language (DSL) used to generate .bmp files containing static ray-casted images of Constructive Solid Geometries (CSGs). The main purpose of this language is to create architectural models in the style of Boullée. The techniques used for ray-casting for the DSL are an extension of those pioneered by Scott D. Roth.

The name “Ozymandias” comes from the Percy Bysshe Shelley poem of the same name.


## Grammar
```
expression = intersection-expression
intersection-expression = union-expression { "&" union-expression }* 
union-expression = primary-expression { ("+" | "-") primary-expression }*
primary-expression = SOLID | (expression)
```
