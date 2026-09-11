# Writing Scripts in Perplex

Perplex allows you to write your own custom behaviours for scene entities. To add a script, right click on your content browser panel and select "Create/C Script". To attach
this script to an entity, right click on the entity inspector and select "Add Component/ScriptComponent". Then drag you newly created C Script into the source field. You can
attach the same script to as many entities as you would like. To edit the script, double click the icon and it will automatically open in the default text editor for .c files.

## Entity API

Use `self` to address the current entity attached to the script instance. For example, to get the position of the current entity, use `position(self)`.
Here are some useful endpoints you should know about:

`position` - the local position of the current entity
`rotation` - the local rotation of the current entity
`scale` - the local scale of the current entity
`set_enabled(enabled)` - sets the current entity's enabled state to enabled
`destroy(delay)` - destroys the current entity after delay seconds
`has_tag(tag)` - compares the current entity's tag to tag
`to_perpixel()` - convertes the current entity into a perpixel system

## Scene API