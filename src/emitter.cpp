#include "yaml-cpp/emitter.h"
#include "emitterstate.h"
#include "emitterutils.h"
#include "indentation.h"
#include "yaml-cpp/exceptions.h"
#include <sstream>

namespace YAML
{	
	Emitter::Emitter(): m_pState(new EmitterState)
	{
	}
	
	Emitter::~Emitter()
	{
	}
	
	const char *Emitter::c_str() const
	{
		return m_stream.str();
	}
	
	unsigned Emitter::size() const
	{
		return m_stream.pos();
	}
	
	// state checking
	bool Emitter::good() const
	{
		return m_pState->good();
	}
	
	const std::string Emitter::GetLastError() const
	{
		return m_pState->GetLastError();
	}

	// global setters
	bool Emitter::SetOutputCharset(EMITTER_MANIP value)
	{
		return m_pState->SetOutputCharset(value, FmtScope::Global);
	}

	bool Emitter::SetStringFormat(EMITTER_MANIP value)
	{
		return m_pState->SetStringFormat(value, FmtScope::Global);
	}
	
	bool Emitter::SetBoolFormat(EMITTER_MANIP value)
	{
		bool ok = false;
		if(m_pState->SetBoolFormat(value, FmtScope::Global))
			ok = true;
		if(m_pState->SetBoolCaseFormat(value, FmtScope::Global))
			ok = true;
		if(m_pState->SetBoolLengthFormat(value, FmtScope::Global))
			ok = true;
		return ok;
	}
	
	bool Emitter::SetIntBase(EMITTER_MANIP value)
	{
		return m_pState->SetIntFormat(value, FmtScope::Global);
	}
	
	bool Emitter::SetSeqFormat(EMITTER_MANIP value)
	{
		return m_pState->SetFlowType(GroupType::Seq, value, FmtScope::Global);
	}
	
	bool Emitter::SetMapFormat(EMITTER_MANIP value)
	{
		bool ok = false;
		if(m_pState->SetFlowType(GroupType::Map, value, FmtScope::Global))
			ok = true;
		if(m_pState->SetMapKeyFormat(value, FmtScope::Global))
			ok = true;
		return ok;
	}
	
	bool Emitter::SetIndent(unsigned n)
	{
		return m_pState->SetIndent(n, FmtScope::Global);
	}
	
	bool Emitter::SetPreCommentIndent(unsigned n)
	{
		return m_pState->SetPreCommentIndent(n, FmtScope::Global);
	}
	
	bool Emitter::SetPostCommentIndent(unsigned n)
	{
		return m_pState->SetPostCommentIndent(n, FmtScope::Global);
	}
    
    bool Emitter::SetFloatPrecision(unsigned n)
    {
        return m_pState->SetFloatPrecision(n, FmtScope::Global);
    }

    bool Emitter::SetDoublePrecision(unsigned n)
    {
        return m_pState->SetDoublePrecision(n, FmtScope::Global);
    }

	// SetLocalValue
	// . Either start/end a group, or set a modifier locally
	Emitter& Emitter::SetLocalValue(EMITTER_MANIP value)
	{
		if(!good())
			return *this;
		
		switch(value) {
			case BeginDoc:
				EmitBeginDoc();
				break;
			case EndDoc:
				EmitEndDoc();
				break;
			case BeginSeq:
				EmitBeginSeq();
				break;
			case EndSeq:
				EmitEndSeq();
				break;
			case BeginMap:
				EmitBeginMap();
				break;
			case EndMap:
				EmitEndMap();
				break;
			case Key:
				EmitKey();
				break;
			case Value:
				EmitValue();
				break;
			case TagByKind:
				EmitKindTag();
				break;
			case Newline:
				EmitNewline();
				break;
			default:
				m_pState->SetLocalValue(value);
				break;
		}
		return *this;
	}
	
	Emitter& Emitter::SetLocalIndent(const _Indent& indent)
	{
		m_pState->SetIndent(indent.value, FmtScope::Local);
		return *this;
	}

    Emitter& Emitter::SetLocalPrecision(const _Precision& precision)
    {
        if(precision.floatPrecision >= 0)
            m_pState->SetFloatPrecision(precision.floatPrecision, FmtScope::Local);
        if(precision.doublePrecision >= 0)
            m_pState->SetDoublePrecision(precision.doublePrecision, FmtScope::Local);
        return *this;
    }

	// EmitBeginDoc
	void Emitter::EmitBeginDoc()
	{
		if(!good())
			return;
	}
	
	// EmitEndDoc
	void Emitter::EmitEndDoc()
	{
		if(!good())
			return;
	}

	// EmitBeginSeq
	void Emitter::EmitBeginSeq()
	{
		if(!good())
			return;
	}
	
	// EmitEndSeq
	void Emitter::EmitEndSeq()
	{
		if(!good())
			return;
	}
	
	// EmitBeginMap
	void Emitter::EmitBeginMap()
	{
		if(!good())
			return;
	}
	
	// EmitEndMap
	void Emitter::EmitEndMap()
	{
		if(!good())
			return;	}
	
	// EmitKey
	void Emitter::EmitKey()
	{
		if(!good())
			return;
	}
	
	// EmitValue
	void Emitter::EmitValue()
	{
		if(!good())
			return;
	}

	// EmitNewline
	void Emitter::EmitNewline()
	{
		if(!good())
			return;
	}

	bool Emitter::CanEmitNewline() const
	{
        return false;
	}

	// *******************************************************************************************
	// overloads of Write
	
	Emitter& Emitter::Write(const std::string& str)
	{
		if(!good())
			return *this;
		return *this;
	}

    unsigned Emitter::GetFloatPrecision() const
    {
        return m_pState->GetFloatPrecision();
    }
    
    unsigned Emitter::GetDoublePrecision() const
    {
        return m_pState->GetDoublePrecision();
    }

	const char *Emitter::ComputeFullBoolName(bool b) const
	{
		const EMITTER_MANIP mainFmt = (m_pState->GetBoolLengthFormat() == ShortBool ? YesNoBool : m_pState->GetBoolFormat());
		const EMITTER_MANIP caseFmt = m_pState->GetBoolCaseFormat();
		switch(mainFmt) {
			case YesNoBool:
				switch(caseFmt) {
					case UpperCase: return b ? "YES" : "NO";
					case CamelCase: return b ? "Yes" : "No";
					case LowerCase: return b ? "yes" : "no";
					default: break;
				}
				break;
			case OnOffBool:
				switch(caseFmt) {
					case UpperCase: return b ? "ON" : "OFF";
					case CamelCase: return b ? "On" : "Off";
					case LowerCase: return b ? "on" : "off";
					default: break;
				}
				break;
			case TrueFalseBool:
				switch(caseFmt) {
					case UpperCase: return b ? "TRUE" : "FALSE";
					case CamelCase: return b ? "True" : "False";
					case LowerCase: return b ? "true" : "false";
					default: break;
				}
				break;
			default:
				break;
		}
		return b ? "y" : "n"; // should never get here, but it can't hurt to give these answers
	}

	Emitter& Emitter::Write(bool b)
	{
		if(!good())
			return *this;

		return *this;
	}

	Emitter& Emitter::Write(char ch)
	{
		if(!good())
			return *this;

		return *this;
	}

	Emitter& Emitter::Write(const _Alias& alias)
	{
		if(!good())
			return *this;

		return *this;
	}
	
	Emitter& Emitter::Write(const _Anchor& anchor)
	{
		if(!good())
			return *this;

		return *this;
	}
	
	Emitter& Emitter::Write(const _Tag& tag)
	{
		if(!good())
			return *this;

        
		return *this;
	}

	void Emitter::EmitKindTag()
	{
		Write(LocalTag(""));
	}

	Emitter& Emitter::Write(const _Comment& comment)
	{
		if(!good())
			return *this;

		return *this;
	}

	Emitter& Emitter::Write(const _Null& /*null*/)
	{
		if(!good())
			return *this;

		return *this;
	}

	Emitter& Emitter::Write(const Binary& binary)
	{
		Write(SecondaryTag("binary"));

		if(!good())
			return *this;

		return *this;
	}
}

