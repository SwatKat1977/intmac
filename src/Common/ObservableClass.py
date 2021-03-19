#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


## Observable class.
class ObservableClass(object):

	## Empty class that is a place-holder for the parameters.
	class ParametersClass():
		_observers = {}

		## Notify observers of property change.
		#  @param self The object pointer.
		#  @param property Name of the property.
		#  @param property Value New value of the property.
		def _NotifyObserversOfPropertyChange(self, property, value):
			# Check that the property has observers, if not then ignore.
			if property not in self._observers: return
			
			# Iterate through observers.
			for observer in self._observers[property]:
				observer.NotifyPropertyChange(property, value)


	## Class constructor.
	#  @param self The object pointer.
	def __init__(self):
		self.parameters = self.ParametersClass()


	## Register a new observer for a property.
	#  @param self The object pointer.
	#  @param property Name of the property.
	#  @param property observer Observer object inheriting from ObserverObject.
	def RegisterObserver(self, property, observer):
		# Check to see if the property exists.
		if hasattr(self.parameters, property) == False:
			raise ValueError('Invalid property')

		# If first time the property has been observed then add property to the
		# dictionary.
		if not property in self.parameters._observers.keys():
			self.parameters._observers[property] = []
			
		# The property is in the dictionary, check that the observer hasn't
		# already been registered, if so raise issue.
		else:
			if observer in self.parameters._observers[property]:
				raise ValueError('Duplicate observer')
	
		# Finally, register the observer.
		self.parameters._observers[property].append(observer)


	## Unregister an existing observer for a property.
	#  @param self The object pointer.
	#  @param property Name of the property.
	#  @param property observer Observer object inheriting from ObserverObject.
	def UnregisterObserver(self, property, observer):
		# Check to see if the property exists.
		if hasattr(self.parameters, property) == False:
			raise ValueError('Invalid property')		
	
		if property not in self.parameters._observers: return
		
		if observer not in self.parameters._observers[property]:
			raise ValueError('Observer not registered for property')			
		
		self.parameters._observers[property].remove(observer)
