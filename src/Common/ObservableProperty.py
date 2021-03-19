#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from operator import attrgetter


## Observable property class.
class ObservableProperty(object):

	## Observable property class constructor, it will take then name and then
	#  prepends the object name to it.
	#  @param self The object pointer.
	#  @param property Name of the property.
	def __init__(self, property):
		customAttr = 'parameters.{0}'.format(property)
		self.attr = customAttr


	## Get the value of the observable property.
	#  @param self The object pointer.
	#  @param instance The instance of the class.
	#  @param type Unused.
	def __get__(self, instance, type):
		return self if instance is None else attrgetter(self.attr)(instance)


	## Set the value of the observable property.
	#  @param self The object pointer.
	#  @param instance The instance of the class.
	#  @param value New value of the property.
	def __set__(self, instance, value):
		head, tail = self.attr.rsplit('.', 1)
		obj = attrgetter(head)(instance)
		setattr(obj, tail, value)
		obj._NotifyObserversOfPropertyChange(tail, value)
