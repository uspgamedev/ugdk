
require "ugdk.base"
require "ugdk.util"

local lang_manager = ugdk_base.Engine_reference():language_manager()

function RegisterLanguages ()
  ugdk_base.Engine_reference():language_manager():RegisterLanguage(
    "en_US",
    "text/lang_en.txt"
  )
  ugdk_base.Engine_reference():language_manager():RegisterLanguage(
    "pt_BR",
    "text/lang_pt_br.txt"
  )
end
