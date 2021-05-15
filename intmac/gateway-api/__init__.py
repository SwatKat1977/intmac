
#pylint: disable=wrong-import-position
import asyncio
import os
import sys
from quart import Quart

## Quart application instance
app = Quart(__name__)

@app.before_serving
async def startup() -> None:
    """
    Code executed before Quart has began serving http requests.

    returns:
        None
    """

@app.after_serving
async def shutdown() -> None:
    """
    Code executed after Quart has stopped serving http requests.

    returns:
        None
    """
