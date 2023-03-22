'''
Copyright (C) 2022-2023 delivery-exchange.com
Unauthorized copying of this file, via any medium is strictly prohibited.
Proprietary and confidential.  All Rights Reserveds
'''
import enum

class AuthenticationType(enum.Enum):
    """ Enumeration for account authentication type """
    GOOGLE_API = 'GoogleApi'
    FACEBOOK = 'Facebook'
    MANUAL = 'manual'
