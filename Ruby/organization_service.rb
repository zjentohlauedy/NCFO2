class OrganizationService

  def initialize repository, conference_service
    @repository         = repository
    @conference_service = conference_service
  end

  def get organization_id
    organization = @repository.read Organization.new organization_id

    unless organization.nil?
      get_conferences organization
    end

    organization
  end

  def get_conferences organization
    organization_conferences = OrganizationConference.new

    results = @repository.custom_read organization_conferences.build_select_by_organization, organization.organization_id

    unless results.nil? or results.length == 0
      organization.conferences = []

      results.each do |result|
        organization_conferences.from_hash result

        unless organization_conferences.conference_id.nil?
          organization_conferences.conference = @conference_service.get organization_conferences.conference_id
        end

        organization.conferences.push organization_conferences

        organization_conferences = OrganizationConference.new
      end
    end
  end

  def save organization
    save_conferences organization.conferences

    @repository.create organization
  end

  def save_conferences conferences
    return if conferences.nil?

    if conferences.is_a? Array
      conferences.each do |conference|
        save_conference conference
      end
    else
      save_conference conferences
    end
  end

  def save_conference conference
    return if conference.nil?

    @repository.create conference

    unless conference.conference.nil?
      @conference_service.save conference.conference
    end
  end
end
